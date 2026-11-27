/****************************************************************************
 * Copyright (c) 2025, ArborX authors                                       *
 * All rights reserved.                                                     *
 *                                                                          *
 * This file is part of the ArborX library. ArborX is                       *
 * distributed under a BSD 3-clause license. For the licensing terms see    *
 * the LICENSE file in the top-level directory.                             *
 *                                                                          *
 * SPDX-License-Identifier: BSD-3-Clause                                    *
 ****************************************************************************/

#ifndef ARBORX_DETAILS_DBSCAN_VERIFICATION_ROUTINES_HPP
#define ARBORX_DETAILS_DBSCAN_VERIFICATION_ROUTINES_HPP

#include <ArborX_Config.hpp>

#include <kokkos_ext/ArborX_KokkosExtKernelStdAlgorithms.hpp>

#include <Kokkos_Core.hpp>
#include <Kokkos_Profiling_ScopedRegion.hpp>

#include <iostream>
#include <set>
#include <stack>
#include <vector>

#ifdef ARBORX_ENABLE_MPI
#include <detail/ArborX_Distributor.hpp>

#include <mpi.h>
#endif

namespace ArborX::Details
{

namespace
{
struct TupleLabelIndexRankCore
{
  long long label;
  unsigned index;
  int rank;
  bool is_core;
};

} // namespace

template <typename Labels, typename IsCore>
struct IndexRankLabelCoreCallback
{
  Labels _labels;
  int _rank;
  IsCore _is_core;

  template <typename Query, typename Value, typename Output>
  KOKKOS_FUNCTION auto operator()(Query const &, Value const &value,
                                  Output const &out) const
  {
    auto const i = value.index;
    if constexpr (std::is_same_v<IsCore, bool>)
      out({_labels(i), i, _rank, _is_core});
    else
      out({_labels(i), i, _rank, _is_core(i)});
  }
};

template <typename Neighbor>
KOKKOS_FUNCTION void dual_print(char const *msg, Neighbor self)
{
  Kokkos::printf(msg);
  if (self.rank == -1)
    Kokkos::printf(": %d [%lld]\n", self.index, self.label);
  else
    Kokkos::printf(": %d (%d) [%lld]\n", self.index, self.rank, self.label);
}
template <typename Neighbor>
KOKKOS_FUNCTION void dual_print(char const *msg, Neighbor self, Neighbor neigh)
{
  Kokkos::printf(msg);
  if (self.rank == -1)
    Kokkos::printf(": %d [%lld] -> %d [%lld]\n", self.index, self.label,
                   neigh.index, neigh.label);
  else
    Kokkos::printf(": %d (%d) [%lld] -> %d (%d) [%lld]\n", self.index,
                   self.rank, self.label, neigh.index, neigh.rank, neigh.label);
}

// Check that core points have nonnegative indices
template <typename ExecutionSpace, typename Offset, typename Neighbors>
bool verifyCorePointsNonnegativeIndex(ExecutionSpace const &exec_space,
                                      Offset offset, Neighbors neighbors,
                                      bool verbose)
{
  auto const n = offset.size() - 1;

  int num_incorrect;
  Kokkos::parallel_reduce(
      "ArborX::DBSCAN::verify::verify_core_points_nonnegative",
      Kokkos::RangePolicy(exec_space, 0, n),
      KOKKOS_LAMBDA(int i, int &update) {
        auto self = neighbors(offset(i));
        if (self.is_core && self.label < 0)
        {
          ++update;
          if (verbose)
            dual_print("Core point is marked as noise", self);
        }
      },
      num_incorrect);
  return (num_incorrect == 0);
}

// Check that connected core points have same cluster indices
template <typename ExecutionSpace, typename Offset, typename Neighbors>
bool verifyConnectedCorePointsShareIndex(ExecutionSpace const &exec_space,
                                         Offset offset, Neighbors neighbors,
                                         bool verbose)
{
  auto const n = offset.size() - 1;

  int num_incorrect;
  Kokkos::parallel_reduce(
      "ArborX::DBSCAN::verify::verify_connected_core_points",
      Kokkos::RangePolicy(exec_space, 0, n),
      KOKKOS_LAMBDA(int i, int &update) {
        auto const self = neighbors(offset(i));
        if (self.is_core)
        {
          for (auto j = offset(i) + 1; j < offset(i + 1); ++j)
          {
            auto const neigh = neighbors(j);
            if (neigh.is_core && self.label != neigh.label)
            {
              ++update;
              if (verbose)
                dual_print("Connected cores do not belong to the same cluster",
                           self, neigh);
            }
          }
        }
      },
      num_incorrect);
  return (num_incorrect == 0);
}

// Check that noise points have index -1
template <typename ExecutionSpace, typename Offset, typename Neighbors>
bool verifyNoisePoints(ExecutionSpace const &exec_space, Offset offset,
                       Neighbors neighbors, bool verbose)
{
  auto const n = offset.size() - 1;

  int num_incorrect;
  Kokkos::parallel_reduce(
      "ArborX::DBSCAN::verify::verify_connected_border_points",
      Kokkos::RangePolicy(exec_space, 0, n),
      KOKKOS_LAMBDA(int i, int &update) {
        auto const self = neighbors(offset(i));
        if (self.is_core)
          return;
        for (auto j = offset(i) + 1; j < offset(i + 1); ++j)
        {
          auto const neigh = neighbors(j);
          if (neigh.is_core)
            return; // border point
        }

        // Noise points must have index -1
        if (self.label != -1)
        {
          update++;
          if (verbose)
            dual_print("Noise point does not have index -1", self);
        }
      },
      num_incorrect);
  return (num_incorrect == 0);
}

// Check that border points share index with at least one core point
template <typename ExecutionSpace, typename Offset, typename Neighbors>
bool verifyConnectedBorderPoints(ExecutionSpace const &exec_space,
                                 Offset offset, Neighbors neighbors,
                                 bool verbose)
{
  auto const n = offset.size() - 1;

  int num_incorrect;
  Kokkos::parallel_reduce(
      "ArborX::DBSCAN::verify::verify_connected_border_points",
      Kokkos::RangePolicy(exec_space, 0, n),
      KOKKOS_LAMBDA(int i, int &update) {
        auto const self = neighbors(offset(i));
        if (self.is_core)
          return;
        bool is_border = false;
        bool have_shared_core = false;
        for (auto j = offset(i) + 1; j < offset(i + 1); ++j)
        {
          auto const neigh = neighbors(j);
          if (neigh.is_core)
          {
            is_border = true;
            if (self.label == neigh.label)
            {
              have_shared_core = true;
              break;
            }
          }
        }

        // Border point must be connected to a core point
        if (is_border && !have_shared_core)
        {
          update++;
          if (verbose)
            dual_print("Border point does not belong to a cluster", self);
        }
      },
      num_incorrect);
  return (num_incorrect == 0);
}

// Check that border points are marked as noise
template <typename ExecutionSpace, typename Offset, typename Neighbors>
bool verifyIgnoredBorderPoints(ExecutionSpace const &exec_space, Offset offset,
                               Neighbors neighbors, bool verbose)
{
  auto const n = offset.size() - 1;

  int num_incorrect;
  Kokkos::parallel_reduce(
      "ArborX::DBSCAN::verify::verify_connected_border_points",
      Kokkos::RangePolicy(exec_space, 0, n),
      KOKKOS_LAMBDA(int i, int &update) {
        auto const self = neighbors(offset(i));
        if (self.is_core)
          return;
        bool is_border = false;
        for (auto j = offset(i) + 1; j < offset(i + 1); ++j)
        {
          auto const neigh = neighbors(j);
          if (neigh.is_core)
          {
            is_border = true;
            break;
          }
        }

        // Border point must be connected to a core point
        if (is_border && self.label != -1)
        {
          update++;
          if (verbose)
            dual_print("Border point does not have index -1", self);
        }
      },
      num_incorrect);
  return (num_incorrect == 0);
}

// Check that cluster indices are unique
template <typename ExecutionSpace, typename Offset, typename Neighbors>
bool verifyClustersAreUnique(ExecutionSpace const &, Offset offset_device,
                             Neighbors neighbors_device, bool verbose)
{
  int const n = offset_device.size() - 1;

  auto offset =
      Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace{}, offset_device);
  auto neighbors = Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace{},
                                                       neighbors_device);

  // Remove all border points from consideration (noise points are already -1)
  // The idea is that this way if labels were bridged through a border
  // point, we will count them as separate labels but with a shared cluster
  // index, which will fail the unique labels check
  for (int i = 0; i < n; ++i)
  {
    auto &self = neighbors(offset(i));
    if (!self.is_core)
    {
      for (auto j = offset(i); j < offset(i + 1); ++j)
      {
        if (neighbors(j).is_core)
        {
          // The point is a border point
          self.label = -1;
          break;
        }
      }
    }
  }

  // Record all unique cluster indices
  std::set<long long> unique_cluster_indices;
  for (int i = 0; i < n; ++i)
  {
    auto const self = neighbors(offset(i));
    if (self.label != -1)
      unique_cluster_indices.insert(self.label);
  }
  auto num_unique_cluster_indices = unique_cluster_indices.size();

  // Record all cluster indices, assigning a unique index to each (which is
  // different from the original cluster index). This will only use noise and
  // core points (see above).
  unsigned int num_clusters = 0;
  std::set<long long> cluster_sets;
  for (int i = 0; i < n; ++i)
  {
    auto const self = neighbors(offset(i));
    if (self.label >= 0)
    {
      auto id = self.label;
      cluster_sets.insert(id);
      ++num_clusters;

      // DFS search
      std::stack<int> stack;
      stack.push(i);
      while (!stack.empty())
      {
        auto k = stack.top();
        stack.pop();
        if (neighbors(offset(k)).label >= 0)
        {
          neighbors(offset(k)).label = -1;
          for (auto j = offset(k); j < offset(k + 1); ++j)
          {
            auto neigh = neighbors(j);
            if (neigh.is_core || neigh.label == id)
              stack.push(neigh.index);
          }
        }
      }
    }
  }
  if (cluster_sets.size() != num_unique_cluster_indices)
  {
    if (verbose)
      std::cerr << "Number of components does not match\n";
    return false;
  }
  if (num_clusters != num_unique_cluster_indices)
  {
    if (verbose)
      std::cerr << "Cluster IDs are not unique\n";
    return false;
  }

  return true;
}

#ifdef ARBORX_ENABLE_MPI
// A cluster label is a global point ID. Gather all core points with that label
// on the rank owning the ID, then reconstruct the core-point graph there.
// Border points cannot be used as paths between core components.
template <typename ExecutionSpace, typename Points, typename Coordinate,
          typename Labels, typename IsCore>
bool verifyDistributedClustersAreUnique(MPI_Comm comm,
                                        ExecutionSpace const &exec_space,
                                        Points const &points, Coordinate eps,
                                        Labels const &labels,
                                        IsCore const &is_core, bool verbose)
{
  Kokkos::Profiling::ScopedRegion guard(
      "ArborX::DBSCAN::verify::verify_distributed_clusters_are_unique");

  using MemorySpace = typename Points::memory_space;
  using Point = typename Points::value_type;

  int comm_size;
  MPI_Comm_size(comm, &comm_size);
  int comm_rank;
  MPI_Comm_rank(comm, &comm_rank);

  std::uint64_t const n = points.size();
  if (n > std::numeric_limits<int>::max())
    Kokkos::abort("DBSCAN verifier exceeds 32-bit neighbor indices");

  // Step 1: determine the global number of points and
  // the rank offsets for each rank
  std::uint64_t n_global;
  Kokkos::View<std::uint64_t *, MemorySpace> rank_offsets(
      Kokkos::view_alloc(exec_space, Kokkos::WithoutInitializing,
                         "ArborX::DBSCAN::verify::rank_offsets"),
      comm_size + 1);
  {
    std::vector<std::uint64_t> counts(comm_size);
    // clang-tidy produces a false positive:
    //   buffer type 'unsigned long' does not match MPI datatype 'MPI_UINT64_T'
    // NOLINTNEXTLINE(mpi-type-mismatch)
    MPI_Allgather(&n, 1, MPI_UINT64_T, counts.data(), 1, MPI_UINT64_T, comm);

    auto rank_offsets_host = Kokkos::create_mirror_view(rank_offsets);
    rank_offsets_host[0] = 0;
    for (int rank = 0; rank < comm_size; ++rank)
      rank_offsets_host[rank + 1] = rank_offsets_host[rank] + counts[rank];
    Kokkos::deep_copy(exec_space, rank_offsets, rank_offsets_host);

    n_global = rank_offsets_host[comm_size];
  }

  Kokkos::View<int *, MemorySpace> destinations(
      Kokkos::view_alloc(exec_space, Kokkos::WithoutInitializing,
                         "ArborX::DBSCAN::verify::cluster_destinations"),
      n);
  auto export_points =
      KokkosExt::cloneWithoutInitializingNorCopying(exec_space, points);
  auto export_labels =
      KokkosExt::cloneWithoutInitializingNorCopying(exec_space, labels);

  std::uint64_t n_export = 0;
  Kokkos::parallel_scan(
      "ArborX::DBSCAN::verify::route_core_points",
      Kokkos::RangePolicy(exec_space, 0, n),
      KOKKOS_LAMBDA(std::uint64_t i, std::uint64_t & update, bool is_final) {
        if (!is_core(i))
          return;

        if (is_final)
        {
          auto const label = labels(i);
          if (label < 0 || static_cast<std::uint64_t>(label) >= n_global)
            Kokkos::abort("DBSCAN verifier: core cluster ID is out of range");

          auto dest_rank =
              KokkosExt::upper_bound(rank_offsets.data(),
                                     rank_offsets.data() + comm_size + 1,
                                     static_cast<std::uint64_t>(label)) -
              1 - rank_offsets.data();
          if (dest_rank < 0 || dest_rank >= comm_size)
            Kokkos::abort("DBSCAN verifier: cannot identify the rank "
                          "containing the label");

          destinations(update) = dest_rank;
          export_points(update) = points(i);
          export_labels(update) = labels(i);
        }
        ++update;
      },
      n_export);
  Kokkos::resize(exec_space, destinations, n_export);
  Kokkos::resize(exec_space, export_points, n_export);
  Kokkos::resize(exec_space, export_labels, n_export);

  Distributor<MemorySpace> distributor(comm);
  auto const n_imports = distributor.createFromSends(exec_space, destinations);
  Kokkos::resize(exec_space, destinations, 0);

  Kokkos::View<Point *, MemorySpace> import_points(
      Kokkos::view_alloc(exec_space, Kokkos::WithoutInitializing,
                         "ArborX::DBSCAN::verify::import_points"),
      n_imports);
  Kokkos::View<long long *, MemorySpace> import_labels(
      Kokkos::view_alloc(exec_space, Kokkos::WithoutInitializing,
                         "ArborX::DBSCAN::verify::import_labels"),
      n_imports);
  distributor.doPostsAndWaits(exec_space, export_points, import_points);
  distributor.doPostsAndWaits(exec_space, export_labels, import_labels);
  Kokkos::resize(exec_space, export_points, 0);
  Kokkos::resize(exec_space, export_labels, 0);

  int local_success = 1;
  if (n_imports > 0)
  {
    BoundingVolumeHierarchy index(exec_space,
                                  Experimental::attach_indices(import_points));
    Kokkos::View<TupleLabelIndexRankCore *, MemorySpace> neighbors(
        "ArborX::DBSCAN::verify::core_neighbors", 0);
    Kokkos::View<std::uint64_t *, MemorySpace> offsets(
        "ArborX::DBSCAN::verify::core_offsets", 0);
    computeVerificationNeighbors(
        exec_space, index, import_points, eps,
        IndexRankLabelCoreCallback<decltype(import_labels), bool>{
            import_labels, comm_rank, true},
        comm_rank, neighbors, offsets);
    local_success =
        verifyClustersAreUnique(exec_space, offsets, neighbors, verbose);
  }

  int global_success;
  MPI_Allreduce(&local_success, &global_success, 1, MPI_INT, MPI_MIN, comm);
  return global_success;
}
#endif

} // namespace ArborX::Details

#endif
