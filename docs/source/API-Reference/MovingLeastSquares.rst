:doc:`ArborX </index>`


``ArborX::InterpolationMovingLeastSquares``
===========================================
Defined in header `<ArborX_InterpMovingLeastSquares.hpp> <https://github.com/arborx/ArborX/blob/master/src/interpolation/ArborX_InterpMovingLeastSquares.hpp>`_

.. code-block:: C++

    template <typename MemorySpace, typename FloatingCalculationType = double>
    class Interpolation::MovingLeastSquares;

The class ``ArborX::Interpolation::MovingLeastSquares`` allows interpolating from a set of source points to a set of target points
using information from nearest neighbors.


Template parameters
-------------------
* ``MemorySpace``: valid Kokkos memory space used as memory space for the coefficients
* ``FloatingCalculatingType``: type of the coefficients used internally for performing the interpolation


Member functions
----------------
+------------------------------------------------------------------------------+---------------------------------------------------+
| :doc:`(constructor) <MovingLeastSquares%3A%3AMovingLeastSquares>`            | computes the coefficients used in ``interpolate`` |
+------------------------------------------------------------------------------+---------------------------------------------------+
| :doc:`interpolate <MovingLeastSquares%3A%3Ainterpolate>`                     | performs the actual interpolation                 |
+------------------------------------------------------------------------------+---------------------------------------------------+
