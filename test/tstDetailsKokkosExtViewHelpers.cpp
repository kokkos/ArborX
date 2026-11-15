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
#include <kokkos_ext/ArborX_KokkosExtViewHelpers.hpp>
#include <misc/ArborX_Exception.hpp>

#include <Kokkos_Core.hpp>

#include "BoostTest_CUDA_clang_workarounds.hpp"
#include <boost/test/unit_test.hpp>

#include <type_traits>

#define BOOST_TEST_MODULE KokkosExtViewHelpers

namespace tt = boost::test_tools;

BOOST_AUTO_TEST_CASE_TEMPLATE(last_element, DeviceType, ARBORX_DEVICE_TYPES)
{
  using ArborX::Details::KokkosExt::lastElement;
  using ExecutionSpace = typename DeviceType::execution_space;
  ExecutionSpace execution_space;
  Kokkos::View<int *, DeviceType> v("v", 2);
  auto v_host = Kokkos::create_mirror_view(v);
  v_host(0) = 33;
  v_host(1) = 24;
  Kokkos::deep_copy(v, v_host);
  BOOST_TEST(lastElement(execution_space, v) == 24);
  Kokkos::View<int *, DeviceType> w("w", 0);
  BOOST_CHECK_THROW(lastElement(execution_space, w), ArborX::SearchException);
  Kokkos::View<double[1], DeviceType> u("u");
  Kokkos::deep_copy(u, 3.14);
  BOOST_TEST(lastElement(execution_space, u) == 3.14);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(clone_unmanaged, DeviceType, ARBORX_DEVICE_TYPES)
{
  namespace KE = ArborX::Details::KokkosExt;
  typename DeviceType::execution_space space;

  using Managed = Kokkos::View<int **, Kokkos::LayoutRight, DeviceType>;
  using Unmanaged = Kokkos::View<int **, Kokkos::LayoutRight, DeviceType,
                                 Kokkos::MemoryUnmanaged>;

  Managed owner("owner", 2, 3);
  Kokkos::deep_copy(space, owner, 42);
  Unmanaged view(owner.data(), 2, 3);

  auto clone = KE::clone(space, view, "clone");
  using CloneType = decltype(clone);
  static_assert(!CloneType::memory_traits::is_unmanaged);
  static_assert(std::is_same_v<typename CloneType::array_layout,
                               typename Unmanaged::array_layout>);
  static_assert(std::is_same_v<typename CloneType::value_type,
                               typename Unmanaged::value_type>);
  static_assert(std::is_same_v<typename CloneType::device_type,
                               typename Unmanaged::device_type>);
  BOOST_TEST(clone.label() == "clone");
  BOOST_TEST(clone.data() != owner.data());
  BOOST_TEST(clone.extent(0) == 2);
  BOOST_TEST(clone.extent(1) == 3);

  {
    auto clone_host =
        Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace{}, clone);
    for (int i = 0; i < 2; ++i)
      for (int j = 0; j < 3; ++j)
        BOOST_TEST(clone_host(i, j) == 42);
  }

  Kokkos::deep_copy(space, owner, 7);
  Kokkos::deep_copy(space, clone, 19);
  space.fence();
  {
    auto owner_host =
        Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace{}, owner);
    auto clone_host =
        Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace{}, clone);
    for (int i = 0; i < 2; ++i)
      for (int j = 0; j < 3; ++j)
      {
        BOOST_TEST(owner_host(i, j) == 7);
        BOOST_TEST(clone_host(i, j) == 19);
      }
  }

  auto empty_clone = KE::cloneWithoutInitializingNorCopying(space, view);
  using EmptyCloneType = decltype(empty_clone);
  static_assert(!EmptyCloneType::memory_traits::is_unmanaged);
  static_assert(std::is_same_v<typename EmptyCloneType::array_layout,
                               typename Unmanaged::array_layout>);
  static_assert(std::is_same_v<typename EmptyCloneType::value_type,
                               typename Unmanaged::value_type>);
  static_assert(std::is_same_v<typename EmptyCloneType::device_type,
                               typename Unmanaged::device_type>);
  BOOST_TEST(empty_clone.data() != owner.data());
  BOOST_TEST(empty_clone.extent(0) == 2);
  BOOST_TEST(empty_clone.extent(1) == 3);
}
