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

#ifndef ARBORX_TRANSLATE_AND_SCALE_HPP
#define ARBORX_TRANSLATE_AND_SCALE_HPP

#include <ArborX_GeometryTraits.hpp>

#include <Kokkos_Macros.hpp>

namespace ArborX::Details
{
// transformation that maps the unit cube into a new axis-aligned box
// NOTE safe to perform in-place
template <typename PointIn, typename PointOut, typename Box>
  requires(GeometryTraits::is_point_v<PointIn> &&
           GeometryTraits::is_point_v<PointOut> &&
           GeometryTraits::is_box_v<Box>)
KOKKOS_FUNCTION void translateAndScale(PointIn const &in, PointOut &out,
                                       Box const &ref)
{
  constexpr int DIM = GeometryTraits::dimension_v<PointIn>;
  static_assert(GeometryTraits::dimension_v<PointOut> == DIM);
  static_assert(GeometryTraits::dimension_v<Box> == DIM);
  for (int d = 0; d < DIM; ++d)
  {
    auto const a = ref.minCorner()[d];
    auto const b = ref.maxCorner()[d];
    out[d] = (a != b ? (in[d] - a) / (b - a) : 0);
  }
}
} // namespace ArborX::Details

#endif
