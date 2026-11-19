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
#include <kokkos_ext/ArborX_KokkosExtFenceOnExit.hpp>

#include <Kokkos_Core.hpp>

#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace
{
// Kokkos tools callbacks are plain function pointers and cannot capture, so
// the observed state lives in an unnamed namespace.
std::string fence_label;
int fence_count = 0;

void begin_fence_callback(char const *name, uint32_t /*device_id*/,
                          uint64_t * /*handle*/)
{
  if (std::string(name) == fence_label)
    ++fence_count;
}

// Installs the begin-fence callback for the lifetime of the object and
// counts fences carrying the given label.
struct FenceCounter
{
  explicit FenceCounter(std::string label)
  {
    fence_label = std::move(label);
    fence_count = 0;
    Kokkos::Tools::Experimental::set_begin_fence_callback(begin_fence_callback);
  }
  ~FenceCounter()
  {
    Kokkos::Tools::Experimental::set_begin_fence_callback(nullptr);
  }
  FenceCounter(FenceCounter const &) = delete;
  FenceCounter &operator=(FenceCounter const &) = delete;
  static int count() { return fence_count; }
};
} // namespace

namespace KokkosExt = ArborX::Details::KokkosExt;

BOOST_AUTO_TEST_SUITE(FenceOnExit)

BOOST_AUTO_TEST_CASE(deduction_guides)
{
  using KokkosExt::FenceOnExit;
  static_assert(std::is_same_v<decltype(FenceOnExit("x")), FenceOnExit<>>);
  // const char * label converts to std::string
  using Device = Kokkos::DefaultExecutionSpace;
  using Host = Kokkos::DefaultHostExecutionSpace;
  static_assert(std::is_same_v<decltype(FenceOnExit("x", Device{}, Host{})),
                               FenceOnExit<Device, Host>>);
  static_assert(!std::is_copy_constructible_v<FenceOnExit<>>);
  static_assert(!std::is_copy_assignable_v<FenceOnExit<>>);
  static_assert(!std::is_move_constructible_v<FenceOnExit<>>);
  static_assert(!std::is_move_assignable_v<FenceOnExit<>>);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(fences_instance_on_scope_exit, DeviceType,
                              ARBORX_DEVICE_TYPES)
{
  using ExecutionSpace = typename DeviceType::execution_space;
  ExecutionSpace space;

  FenceCounter counter("arborx_test_instance_fence");
  {
    KokkosExt::FenceOnExit guard("arborx_test_instance_fence", space);
    BOOST_TEST(counter.count() == 0);
  }
  BOOST_TEST(counter.count() == 1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(fences_every_instance, DeviceType,
                              ARBORX_DEVICE_TYPES)
{
  using ExecutionSpace = typename DeviceType::execution_space;
  ExecutionSpace space1;
  ExecutionSpace space2;

  FenceCounter counter("arborx_test_multi_fence");
  {
    KokkosExt::FenceOnExit guard("arborx_test_multi_fence", space1, space2);
    BOOST_TEST(counter.count() == 0);
  }
  BOOST_TEST(counter.count() == 2);
}

BOOST_AUTO_TEST_CASE(fences_globally_without_spaces)
{
  FenceCounter counter("arborx_test_global_fence");
  {
    KokkosExt::FenceOnExit guard("arborx_test_global_fence");
    BOOST_TEST(counter.count() == 0);
  }
  // Global fence is forwarded to every enabled backend
  BOOST_TEST(counter.count() >= 1);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(fences_when_unwinding, DeviceType,
                              ARBORX_DEVICE_TYPES)
{
  using ExecutionSpace = typename DeviceType::execution_space;
  ExecutionSpace space;

  FenceCounter counter("arborx_test_exception_fence");
  BOOST_CHECK_THROW(
      {
        KokkosExt::FenceOnExit guard("arborx_test_exception_fence", space);
        throw std::runtime_error("boom");
      },
      std::runtime_error);
  BOOST_TEST(counter.count() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
