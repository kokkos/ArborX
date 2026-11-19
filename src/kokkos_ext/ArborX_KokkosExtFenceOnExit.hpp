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

#ifndef ARBORX_KOKKOS_EXT_FENCE_ON_EXIT_HPP
#define ARBORX_KOKKOS_EXT_FENCE_ON_EXIT_HPP

#include <Kokkos_Core.hpp>

#include <string>
#include <tuple>
#include <utility>

namespace ArborX::Details::KokkosExt
{

// RAII guard that fences on scope exit.
//
// Typical use: asynchronous operations on an execution space read from or
// write to a host buffer. Declaring the guard *after* the buffer guarantees
// that (by reverse order of destruction) the fence happens before the buffer
// is released, on every exit path including exceptions.
//
//   FenceOnExit g("label");            // global fence
//   FenceOnExit g("label", e1, e2);    // fence the given instances
template <class... ExecSpaces>
class FenceOnExit
{
  static_assert((Kokkos::is_execution_space_v<ExecSpaces> && ...),
                "All template arguments must be Kokkos execution spaces");

  std::tuple<ExecSpaces...> _spaces;
  std::string _label;

public:
  explicit FenceOnExit(std::string label, ExecSpaces... spaces)
      : _spaces(std::move(spaces)...)
      , _label(std::move(label))
  {}

  FenceOnExit(FenceOnExit const &) = delete;
  FenceOnExit &operator=(FenceOnExit const &) = delete;

  ~FenceOnExit()
  {
    if constexpr (sizeof...(ExecSpaces) == 0)
      Kokkos::fence(_label); // no spaces -> global fence
    else
      std::apply([this](auto &...s) { (s.fence(_label), ...); }, _spaces);
  }
};

// FenceOnExit g("x");         -> FenceOnExit<>        (global)
// FenceOnExit g("x", e1, e2); -> FenceOnExit<E1, E2>  (per-instance)
template <class... ES>
FenceOnExit(std::string, ES...) -> FenceOnExit<ES...>;

} // namespace ArborX::Details::KokkosExt

#endif
