``MovingLeastSquares``
======================
Defined in header `<ArborX_InterpMovingLeastSquares.hpp> <https://github.com/arborx/ArborX/blob/master/src/interpolation/ArborX_InterpMovingLeastSquares.hpp>`_

.. cpp:class:: template <typename MemorySpace, \
                         typename FloatingCalculationType = double> \
               Interpolation::MovingLeastSquares

   Interpolate values from a set of source points to a set of target points
   using the moving least squares method.

   For each target point, a polynomial is fitted by weighted least squares to
   the values at its nearest source points, and evaluated at the target point.
   The weights are given by a compactly supported radial basis function, see
   `Compact radial basis functions`_.
   All the work that depends on the positions of the points only is done in the
   constructor.
   The interpolation itself, :cpp:func:`interpolate()
   <Interpolation::MovingLeastSquares::interpolate>`, is a weighted sum of
   source values and can be repeated for as many sets of values as needed.

   :tparam MemorySpace: A valid Kokkos memory space, in which the coefficients
      reside.
   :tparam FloatingCalculationType: The floating-point type used internally to
      compute and store the coefficients.

   .. cpp:function:: template <typename ExecutionSpace, typename SourcePoints, \
                               typename TargetPoints, \
                               typename CRBFunc = CRBF::Wendland<0>, \
                               typename PolynomialDegree = PolynomialDegree<2>> \
                     MovingLeastSquares(ExecutionSpace const &space, \
                                        SourcePoints const &source_points, \
                                        TargetPoints const &target_points, \
                                        CRBFunc = {}, PolynomialDegree = {}, \
                                        std::optional<int> num_neighbors = std::nullopt)

      Compute the coefficients of the interpolation between the source points
      and the target points.

      :param space: The execution space in which to run the algorithm.
      :param source_points: The points at which the values to interpolate are
         known, accessed through :cpp:struct:`AccessTraits`.
      :param target_points: The points at which to interpolate, accessed
         through :cpp:struct:`AccessTraits`.
      :param CRBFunc: The compact radial basis function used to weight the
         source points, see `Compact radial basis functions`_.
         The default is :cpp:struct:`CRBF::Wendland\<0\> <Interpolation::CRBF::Wendland>`.
      :param PolynomialDegree: The degree of the polynomial basis, see
         `Polynomial degree`_.
         The default is a degree of 2.
      :param num_neighbors: The number of source points, among the nearest
         neighbors of a target point, used to interpolate at that target point.
         It defaults to the size of the polynomial basis.

      The following requirements must be satisfied.

      * ``MemorySpace`` must be accessible from ``ExecutionSpace``.
      * A specialization of :cpp:struct:`AccessTraits` must match
        ``SourcePoints`` and ``TargetPoints`` as the template argument, and
        :cpp:func:`AccessTraits::get` must return a
        :doc:`point </API-Reference/Geometries%3A%3APoint>`.
      * The source points and the target points must have the same dimension
        and be accessible from ``ExecutionSpace``.
      * ``num_neighbors`` must be positive and at most the number of source
        points.

      The interpolation is exact, up to machine precision, for any function in
      the span of the polynomial basis, provided the Vandermonde matrix of the
      basis evaluated at the neighbors of the target point has full rank.
      Otherwise, the higher-order terms are dropped from the polynomial basis
      for that target point, which gives a better interpolation of lower-order
      polynomials.

   .. cpp:function:: template <typename ExecutionSpace, typename SourceValues, \
                               typename ApproxValues> \
                     void interpolate(ExecutionSpace const &space, \
                                      SourceValues const &source_values, \
                                      ApproxValues &approx_values) const

      Compute the interpolated values at the target points from the values at
      the source points, using the coefficients computed by the constructor.
      It may be called any number of times.

      :param space: The execution space in which to run the algorithm.
      :param source_values: The values at the source points.
      :param approx_values: The view in which the interpolated values at the
         target points are stored.

      The following requirements must be satisfied.

      * ``MemorySpace`` must be accessible from ``ExecutionSpace``.
      * ``SourceValues`` and ``ApproxValues`` must be one-dimensional Kokkos
        views accessible from ``ExecutionSpace``, and the value type of
        ``ApproxValues`` must not be ``const``.
      * ``source_values`` must have one entry per source point, in the order of
        the ``source_points`` passed to the constructor.
      * ``approx_values`` must have one entry per target point, in the order of
        the ``target_points`` passed to the constructor.


Compact radial basis functions
------------------------------
A compact radial basis function (CRBF) is a function of the distance to the
target point, rescaled so that the farthest neighbor is slightly inside the
support of the function, which is the unit ball.
Source points get a weight that decreases to zero as they get farther away.
The ``CRBFunc`` argument of the constructor is one of the following types, each
parameterized by a smoothness-related integer ``N``.

.. cpp:struct:: template <std::size_t N> Interpolation::CRBF::Wendland

   The Wendland functions [1]_ are available for ``N`` in 0, 2, 4, and 6.
   For instance, ``Wendland<0>`` is ``(1 - x)^2`` and ``Wendland<2>`` is
   ``(1 - x)^4 (4x + 1)``, for ``x`` in [0, 1].

.. cpp:struct:: template <std::size_t N> Interpolation::CRBF::Wu

   The Wu functions [2]_ are available for ``N`` in 2 and 4.
   For instance, ``Wu<2>`` is ``(1 - x)^4 (3x^3 + 12x^2 + 16x + 4)``, for ``x``
   in [0, 1].

.. cpp:struct:: template <std::size_t N> Interpolation::CRBF::Buhmann

   The Buhmann functions [3]_ are available for ``N`` in 2, 3, and 4.

Each of these has a static member function that evaluates the function.
Its argument is clamped to [0, 1] after taking its absolute value, and ``DIM`` is
the dimension of the points, as the functions depend on it.

.. cpp:function:: template <std::size_t DIM, typename T> \
                  static constexpr T Interpolation::CRBF::Wendland::evaluate(T const x)
   :noindexentry:

   Evaluate the function.


Polynomial degree
-----------------
.. cpp:type:: template <std::size_t Degree> \
              Interpolation::PolynomialDegree = std::integral_constant<std::size_t, Degree>

   The degree of the polynomial basis, which contains all the monomials of the
   coordinates of the points up to that degree.
   For points of dimension ``d``, it has :math:`\binom{d + Degree}{d}` terms,
   e.g., 6 in 2D and 10 in 3D for a degree of 2.

Use it as, e.g., ``Interpolation::PolynomialDegree<1>{}``.


Example
-------
The example below interpolates :math:`\cos(x + y / 4)` from a 3x3 grid of
points to three points that are not on the grid.
The default compact radial basis function and polynomial degree are used.

.. literalinclude:: ../../../../examples/moving_least_squares/moving_least_squares.cpp
   :language: C++
   :lines: 11-

It prints

.. code-block:: text

   Approximated values: 0.503076 -0.156426 0.87675
   Real values        : 0.540302 -0.178246 0.914443
   Differences        : 0.0372265 0.0218204 0.0376936


References
----------
.. [1] Wendland, H., "`Piecewise polynomial, positive definite and compactly
   supported radial functions of minimal degree
   <https://doi.org/10.1007/BF02123482>`_".
   Advances in Computational Mathematics 4, pp. 389-396. 1995

.. [2] Wu, Z., "`Compactly supported positive definite radial functions
   <https://doi.org/10.1007/BF03177517>`_".
   Advances in Computational Mathematics 4, pp. 283-292. 1995

.. [3] Buhmann, M. D., "`A new class of radial basis functions with compact
   support <https://doi.org/10.1090/S0025-5718-00-01251-5>`_".
   Mathematics of Computation 70(233), pp. 307-318. 2001

