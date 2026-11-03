:orphan:

:doc:`ArborX </index>` / :doc:`ArborX::Interpolation::MovingLeastSquares <MovingLeastSquares>`


``ArborX::Interpolation::MovingLeastSquares<MemorySpace, FloatingCalculationType>::MovingLeastSquares``
=======================================================================================================
.. code-block:: C++

    template <typename ExecutionSpace, typename SourcePoints,
              typename TargetPoints, typename CRBFunc = CRBF::Wendland<0>,
              typename PolynomialDegree = PolynomialDegree<2>>
    MovingLeastSquares(ExecutionSpace const &space,
                       SourcePoints const &source_points,
                       TargetPoints const &target_points, CRBFunc = {},
                       PolynomialDegree = {},
                       std::optional<int> num_neighbors = std::nullopt); // (1)

1) Constructor computing the coefficients for the interpolation between source and target points.


Parameters
----------
``space``: an execution space instance that can access MemorySpace

``source_points``: rank-1 Kokkos::View containing element of type ``ArborX::Point`` representing the source points accessible from ``space``

``target_points``: rank-1 Kokkos::View containing element of type ``ArborX::Point`` representing the target points accessible from ``space``

``CRBFunc``: type of compact radial basis functions to be used in the moving least squares algorithm

``PolynomialDegree``: degree of the polynomial basis

``num_neighbors``: number of source point neighbors considered for every target point defaulting to the size of the polynomial basis


Notes
-----
The interpolation will be exact (up to machine precision) for interpolants contained in the polynomial ansatz space
provided that the Vandermonde matrix constructed from the evaluation of the polynomila basis in the nearest neigbor source points
has full rank. Otherwise, higher-order polynomials will be dropped from the polynomial basis. This ensures better interpolation for
lower-order polynomials.


Example
-------
.. code-block:: C++

    #include <ArborX.hpp>
    #include <ArborX_InterpMovingLeastSquares.hpp>

    #include <Kokkos_Core.hpp>

    using Point = ArborX::Point<2, double>;

    int main(int argc, char *argv[])
    {
      Kokkos::ScopeGuard guard(argc, argv);

      using ExecutionSpace = Kokkos::DefaultExecutionSpace;
      using MemorySpace = typename ExecutionSpace::memory_space;
      ExecutionSpace space{};

      int const num_sources = 9;
      int const num_targets = 3;

      Kokkos::View<Point *, MemorySpace> src_points("Example::src_points",
                                                    num_sources);
      Kokkos::View<Point *, MemorySpace> tgt_points("Example::tgt_points",
                                                    num_targets);
      // fill src_points and tgt_points
      // ...

      // Build the moving least squares coefficients
      ArborX::Interpolation::MovingLeastSquares<MemorySpace> mls(space, src_points,
                                                                 tgt_points);
    }
