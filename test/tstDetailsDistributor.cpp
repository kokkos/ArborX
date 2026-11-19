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
#include "ArborX_EnableDeviceTypes.hpp" // ARBORX_DEVICE_TYPES
#include "ArborX_EnableViewComparison.hpp"
#include <detail/ArborX_DistributedTreeUtils.hpp>
#include <kokkos_ext/ArborX_KokkosExtStdAlgorithms.hpp>

#include <boost/test/unit_test.hpp>

#define BOOST_TEST_MODULE DetailsDistributor

namespace tt = boost::test_tools;

template <typename DeviceType>
struct Helper
{
  template <typename View1, typename View2, typename View3>
  static void checkDoPostsAndWaits(MPI_Comm comm, View1 const &ranks,
                                   View2 const &exports,
                                   View3 const &imports_ref)
  {
    ArborX::Details::Distributor<DeviceType> distributor(comm);
    distributor.createFromSends(typename DeviceType::execution_space{}, ranks);

    // NOTE here we assume that the reference solution is sized properly
    auto imports =
        Kokkos::create_mirror(typename View2::memory_space(), imports_ref);

    distributor.doPostsAndWaits(typename DeviceType::execution_space{}, exports,
                                imports);

    auto imports_host = Kokkos::create_mirror_view(imports);
    Kokkos::deep_copy(imports_host, imports);
    auto imports_ref_host = Kokkos::create_mirror_view(imports_ref);
    Kokkos::deep_copy(imports_ref_host, imports_ref);

    BOOST_TEST(imports_host == imports_ref_host, tt::per_element());
  }
};

BOOST_AUTO_TEST_CASE_TEMPLATE(do_posts_and_waits, DeviceType,
                              ARBORX_DEVICE_TYPES)
{
  using ExecutionSpace = typename DeviceType::execution_space;

  MPI_Comm comm = MPI_COMM_WORLD;
  int comm_rank;
  MPI_Comm_rank(comm, &comm_rank);
  int comm_size;
  MPI_Comm_size(comm, &comm_size);

  // send 1 packet to rank k
  // receive comm_size packets
  Kokkos::View<int *, DeviceType> exports("Testing::exports", comm_size);
  Kokkos::parallel_for(
      Kokkos::RangePolicy<ExecutionSpace>(0, comm_size),
      KOKKOS_LAMBDA(int i) { exports(i) = (i < comm_rank ? 0 : comm_rank); });

  Kokkos::View<int *, DeviceType> ranks("Testing::ranks", comm_size);
  ArborX::Details::KokkosExt::iota(ExecutionSpace{}, ranks, 0);

  Kokkos::View<int *, DeviceType> imports_ref("Testing::v_ref", comm_size);
  Kokkos::parallel_for(
      Kokkos::RangePolicy<ExecutionSpace>(0, comm_size), KOKKOS_LAMBDA(int i) {
        // Sum of all smaller ranks including this one
        imports_ref(i) = (i <= comm_rank ? i : 0);
      });

  Helper<DeviceType>::checkDoPostsAndWaits(comm, ranks, exports, imports_ref);

  Kokkos::View<int *, DeviceType, Kokkos::MemoryUnmanaged> exports_unmanaged{
      exports};
  Helper<DeviceType>::checkDoPostsAndWaits(comm, ranks, exports_unmanaged,
                                           imports_ref);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(large_batched_counts, DeviceType,
                              ARBORX_DEVICE_TYPES)
{
  using ExecutionSpace = typename DeviceType::execution_space;

  int comm_size;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  int comm_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

  Kokkos::View<int *, DeviceType> ranks("Testing::large_ranks", 2);
  auto ranks_host = Kokkos::create_mirror_view(ranks);
  ranks_host(0) = comm_rank;
  ranks_host(1) = (comm_rank + 1) % comm_size;
  Kokkos::deep_copy(ranks, ranks_host);

  // Total may exceeed 32 bit, but no individual send does.
  Kokkos::View<std::uint64_t *, DeviceType> offsets("Testing::large_offsets",
                                                    3);
  auto offsets_host = Kokkos::create_mirror_view(offsets);
  std::uint64_t const count = comm_size > 1 ? std::uint64_t{INT32_MAX} + 3
                                            : std::uint64_t{INT32_MAX} - 1;
  offsets_host(0) = 0;
  offsets_host(1) = count / 2;
  offsets_host(2) = count;
  Kokkos::deep_copy(offsets, offsets_host);

  ArborX::Details::Distributor<DeviceType> distributor(MPI_COMM_WORLD);
  BOOST_TEST(distributor.createFromSends(ExecutionSpace{}, ranks, offsets) ==
             count);
  BOOST_TEST(distributor.getTotalSendLength() == count);
  BOOST_TEST(distributor.getTotalReceiveLength() == count);
}
