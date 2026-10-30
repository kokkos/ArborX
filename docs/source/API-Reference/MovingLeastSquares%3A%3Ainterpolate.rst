:orphan:

:doc:`ArborX </index>` / :doc:`ArborX::Interpolation::MovingLeastSquares <MovingLeastSquares>`


``ArborX::Interpolation::MovingLeastSquares<MemorySpace, FloatingCalculationType>::interpolate``
================================================================================================
.. code-block:: C++

      template <typename ExecutionSpace, typename SourceValues,
                typename TargetValues>
      void interpolate(ExecutionSpace const &space,
                       SourceValues const &source_values,
                       TargetValues &approx_values) const

1) Computes the interpolated target values given the source values using the coefficients computed in the constructor call.
   Can be invoked multiple times.


Parameters
----------
``space``: an execution space instance that can access MemorySpace

``source_values``: rank-1 Kokkos::View containing values in the source points provided in the constructor call

``approx_values``: rank-1 Kokkos::View storing the interpolated values in the target points provided in the constructor call


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
      Kokkos::View<Point *, MemorySpace> tgrt_points("Example::tgrt_points",
                                                    num_targets);
      // fill src_points and tgrt_points
      // ...

      // Build the moving least squares coefficients
      ArborX::Interpolation::MovingLeastSquares<MemorySpace> mls(space, src_points,
                                                                 tgrt_points);

      Kokkos::View<double *, MemorySpace> src_values("Example::src_values",
                                                     num_sources);
      Kokkos::View<double *, MemorySpace> trgt_values("Example::trgt_values",
                                                     num_targets);
      // fill source values
      // ...

      // Interpolate
      mls.interpolate(space, src_values, trgt_values);

      // do something with the interpolated values
      // ...
    }
