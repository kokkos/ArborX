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

#ifndef ARBORX_DETAILSDBSCANVERIFICATION_HPP
#define ARBORX_DETAILSDBSCANVERIFICATION_HPP

#include <ArborX_Config.hpp>

#include <ArborX_DBSCAN.hpp>
#include <ArborX_LinearBVH.hpp>
#ifdef ARBORX_ENABLE_MPI
#include <ArborX_DistributedTree.hpp>

#include <mpi.h>
#endif
#include <ArborX_DBSCANVerificationRoutines.hpp>

#include <Kokkos_Core.hpp>
#include <Kokkos_Profiling_ScopedRegion.hpp>
#include <Kokkos_Swap.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

namespace ArborX::Details
{

template <typename ExecutionSpace, typename Tree, typename Points,
          typename Coordinate, typename Callback, typename Neighbors,
          typename Offsets>
void computeVerificationNeighbors(ExecutionSpace const &exec_space,
                                  Tree const &index, Points const &points,
                                  Coordinate eps, Callback const &callback,
                                  int rank, Neighbors &neighbors,
                                  Offsets &offsets)
{
  index.query(exec_space, Experimental::make_intersects(points, eps), callback,
              neighbors, offsets);
  Kokkos::parallel_for(
      "ArborX::Details::computeVerificationNeighbors::place_self_first",
      Kokkos::RangePolicy(exec_space, 0, points.size()),
      KOKKOS_LAMBDA(unsigned i) {
        typename Offsets::non_const_value_type self_index;
        for (auto j = offsets(i); j < offsets(i + 1); ++j)
          if (neighbors(j).index == i && neighbors(j).rank == rank)
            self_index = j;

        KOKKOS_ASSERT(self_index >= 0);
        if (self_index != offsets(i))
          Kokkos::kokkos_swap(neighbors(offsets(i)), neighbors(self_index));
      });
}

template <typename ExecutionSpace, Details::Concepts::Primitives Primitives,
          typename Labels, typename Coordinate>
bool verifyDBSCAN(ExecutionSpace exec_space, Primitives const &primitives,
                  Coordinate eps, int core_min_size, Labels const &labels,
                  DBSCAN::Algorithm algorithm = DBSCAN::Algorithm::DBSCAN,
                  bool verbose = false)
{
  Kokkos::Profiling::ScopedRegion guard("ArborX::DBSCAN::verify");

  static_assert(Kokkos::is_view<Labels>{});

  using Points = Details::AccessValues<Primitives>;
  using MemorySpace = typename Points::memory_space;

  static_assert(std::is_same_v<typename Labels::value_type, int>);
  static_assert(std::is_same_v<typename Labels::memory_space, MemorySpace>);

  ARBORX_ASSERT(eps > 0);
  ARBORX_ASSERT(core_min_size >= 2);

  Points points{primitives}; // NOLINT
  auto const n = points.size();

  constexpr int rank = -1; // single rank

  using Point = typename Points::value_type;
  static_assert(GeometryTraits::is_point_v<Point>);

  ArborX::BoundingVolumeHierarchy index(
      exec_space, ArborX::Experimental::attach_indices(points));

  Kokkos::View<TupleLabelIndexRankCore *, MemorySpace> neighbors(
      "ArborX::DBSCAN::verify::neighbors", 0);
  Kokkos::View<int *, MemorySpace> offset("ArborX::DBSCAN::verify::offset", 0);
  computeVerificationNeighbors(
      exec_space, index, points, eps,
      IndexRankLabelCoreCallback<Labels, bool>{labels, rank, false /*is_core*/},
      rank, neighbors, offset);

  Kokkos::parallel_for(
      "ArborX::DBSCAN::verify::set_is_core",
      Kokkos::RangePolicy(exec_space, 0, n), KOKKOS_LAMBDA(unsigned i) {
        for (int jj = offset(i); jj < offset(i + 1); ++jj)
        {
          auto j = neighbors(jj).index;
          neighbors(jj).is_core = (offset(j + 1) - offset(j) >= core_min_size);
        }
      });

  using Verify = bool (*)(ExecutionSpace const &, decltype(offset),
                          decltype(neighbors), bool);

  std::vector<Verify> verify{
      static_cast<Verify>(verifyCorePointsNonnegativeIndex),
      static_cast<Verify>(verifyConnectedCorePointsShareIndex),
      static_cast<Verify>(verifyNoisePoints),
      (algorithm == DBSCAN::Algorithm::DBSCAN
           ? static_cast<Verify>(verifyConnectedBorderPoints)
           : static_cast<Verify>(verifyIgnoredBorderPoints)),
      static_cast<Verify>(verifyClustersAreUnique)};
  return std::all_of(verify.begin(), verify.end(), [&](Verify const &verify) {
    return verify(exec_space, offset, neighbors, verbose);
  });
}

#ifdef ARBORX_ENABLE_MPI
template <typename ExecutionSpace, Details::Concepts::Primitives Primitives,
          typename Labels, typename Coordinate>
bool verifyDBSCAN(MPI_Comm comm, ExecutionSpace exec_space,
                  Primitives const &primitives, Coordinate eps,
                  int core_min_size, Labels const &labels, bool verbose = false)
{
  Kokkos::Profiling::ScopedRegion guard("ArborX::DBSCAN::verify");

  static_assert(Kokkos::is_view<Labels>{});

  using Points = Details::AccessValues<Primitives>;
  using MemorySpace = typename Points::memory_space;

  static_assert(std::is_same_v<typename Labels::value_type, long long>);
  static_assert(std::is_same_v<typename Labels::memory_space, MemorySpace>);

  ARBORX_ASSERT(eps > 0);
  ARBORX_ASSERT(core_min_size >= 2);

  Points points{primitives}; // NOLINT
  std::uint64_t const n = points.size();
  if (n > std::numeric_limits<int>::max())
    Kokkos::abort("DBSCAN verifier exceeds 32-bit neighbor indices");

  using Point = typename Points::value_type;
  static_assert(GeometryTraits::is_point_v<Point>);

  // FIXME_NVCC: nvcc 11.7 (and maybe others) cannot compile this without
  // template arguments.
  ArborX::DistributedTree<MemorySpace,
                          PairValueIndex<typename Points::value_type, unsigned>>
      index(comm, exec_space, ArborX::Experimental::attach_indices(points));

  // Phase 1: determine core points by getting offset
  //
  // We do it differently from on-node case, because we will also need to
  // communicate that status.
  Kokkos::View<bool *, MemorySpace> is_core(
      Kokkos::view_alloc(exec_space, Kokkos::WithoutInitializing,
                         "ArborX::DBSCAN::verify::is_core"),
      n);
  {
    // TODO: change the callback to a distributed CountUpToN equivalent
    // to speed things up (probably using post-callbacks)
    Kokkos::View<std::uint64_t *, MemorySpace> offset(
        "ArborX::DBSCAN::verify::offset", 0);
    Kokkos::View<unsigned *, MemorySpace> indices(
        "ArborX::DBSCAN::verify::indices", 0);
    index.query(exec_space, ArborX::Experimental::make_intersects(points, eps),
                Experimental::ExtractPairIndexCallback{}, indices, offset);

    Kokkos::parallel_for(
        "ArborX::DBSCAN::verify::set_is_core",
        Kokkos::RangePolicy(exec_space, 0, n), KOKKOS_LAMBDA(int i) {
          is_core(i) = (offset(i + 1) - offset(i) >=
                        static_cast<std::uint64_t>(core_min_size));
        });
  }

  int rank;
  MPI_Comm_rank(comm, &rank);

  // Phase 2: return point information (index, rank, label, core) from all ranks
  Kokkos::View<TupleLabelIndexRankCore *, MemorySpace> neighbors(
      "ArborX::DBSCAN::verify::neighbors", 0);
  Kokkos::View<std::uint64_t *, MemorySpace> offset(
      "ArborX::DBSCAN::verify::offset", 0);
  computeVerificationNeighbors(
      exec_space, index, points, eps,
      IndexRankLabelCoreCallback<Labels, decltype(is_core)>{labels, rank,
                                                            is_core},
      rank, neighbors, offset);

  using Verify = bool (*)(ExecutionSpace const &, decltype(offset),
                          decltype(neighbors), bool);

  std::vector<Verify> verify{
      static_cast<Verify>(verifyCorePointsNonnegativeIndex),
      static_cast<Verify>(verifyConnectedCorePointsShareIndex),
      static_cast<Verify>(verifyNoisePoints),
      static_cast<Verify>(verifyConnectedBorderPoints)};
  int local_success =
      std::all_of(verify.begin(), verify.end(), [&](Verify const &verify) {
        return verify(exec_space, offset, neighbors, verbose);
      });

  int global_success;
  MPI_Allreduce(&local_success, &global_success, 1, MPI_INT, MPI_MIN, comm);

  // All ranks must make the same decision before the uniqueness check enters
  // its collective redistribution.
  if (!global_success)
    return false;

  return verifyDistributedClustersAreUnique(comm, exec_space, points, eps,
                                            labels, is_core, verbose);
}
#endif

} // namespace ArborX::Details

#endif
