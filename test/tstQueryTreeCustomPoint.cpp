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
#include <ArborX_LinearBVH.hpp>
#include <ArborX_Point.hpp>
#include <detail/ArborX_Iota.hpp>

#include <Kokkos_Core.hpp>

#include <boost/test/unit_test.hpp>

#include <vector>

#include "Search_UnitTestHelpers.hpp"

struct CustomPoint
{
  float y;
  float x;
  KOKKOS_FUNCTION float operator[](int i) const { return i == 0 ? x : 2 * y; }
};

template <>
struct ArborX::GeometryTraits::dimension<CustomPoint>
    : std::integral_constant<int, 2>
{};
template <>
struct ArborX::GeometryTraits::tag<CustomPoint>
{
  using type = ArborX::GeometryTraits::PointTag;
};
template <>
struct ArborX::GeometryTraits::coordinate_type<CustomPoint>
{
  using type = float;
};

BOOST_AUTO_TEST_CASE_TEMPLATE(bvh_custom_point_construction, DeviceType,
                              ARBORX_DEVICE_TYPES)
{
  using ExecutionSpace = typename DeviceType::execution_space;
  using MemorySpace = typename DeviceType::memory_space;

  using Box = ArborX::Box<2>;

  ExecutionSpace space;

  std::vector<CustomPoint> points_v = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
  auto const n = points_v.size();

  Kokkos::View<CustomPoint *, MemorySpace> points("Testing::primitives", n);
  Kokkos::deep_copy(
      space, points,
      Kokkos::View<CustomPoint *, Kokkos::HostSpace, Kokkos::MemoryUnmanaged>(
          points_v.data(), points_v.size()));
  ArborX::BoundingVolumeHierarchy const tree(
      space, ArborX::Details::Iota<MemorySpace>(n), points);

  Kokkos::View<ArborX::Intersects<Box> *, MemorySpace> queries(
      "Testing::queries", 1);
  Kokkos::deep_copy(queries, ArborX::intersects(Box{{-0.1, 1.9}, {1.1, 2.1}}));
  ARBORX_TEST_QUERY_TREE(space, tree, queries,
                         make_reference_solution<int>({2, 3}, {0, 2}));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(bvh_custom_point_query, DeviceType,
                              ARBORX_DEVICE_TYPES)
{
  using ExecutionSpace = typename DeviceType::execution_space;
  using MemorySpace = typename DeviceType::memory_space;

  using Box = ArborX::Box<2>;

  ExecutionSpace space;

  std::vector<Box> boxes_v = {
      {{0, 0}, {1, 1}},
      {{1, 0}, {2, 1}},
      {{0, 1}, {1, 2}},
      {{1, 1}, {2, 2}},
  };
  auto const n = boxes_v.size();

  Kokkos::View<Box *, MemorySpace> boxes("Testing::primitives", n);
  Kokkos::deep_copy(
      space, boxes,
      Kokkos::View<Box *, Kokkos::HostSpace, Kokkos::MemoryUnmanaged>(
          boxes_v.data(), boxes_v.size()));
  ArborX::BoundingVolumeHierarchy const tree(
      space, ArborX::Details::Iota<MemorySpace>(n), boxes);

  Kokkos::View<ArborX::Intersects<CustomPoint> *, MemorySpace> queries(
      "Testing::queries", 1);
  Kokkos::deep_copy(queries, ArborX::intersects(CustomPoint{1.5, 1}));
  ARBORX_TEST_QUERY_TREE(space, tree, queries,
                         make_reference_solution<int>({}, {0, 0}));

  Kokkos::deep_copy(queries, ArborX::intersects(CustomPoint{0.6, 1.5}));
  ARBORX_TEST_QUERY_TREE(space, tree, queries,
                         make_reference_solution<int>({3}, {0, 1}));
}
