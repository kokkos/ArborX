:orphan:

:doc:`ArborX </index>` / :doc:`Geometries <Geometries>` / :doc:`ArborX::Box <Geometries%3A%3ABox>`


``ArborX::Box<DIM, Coordinate>::Box``
=====================================
.. code-block:: C++

    KOKKOS_FUNCTION
    constexpr Box(); // (1)

    KOKKOS_FUNCTION
    constexpr Box(ArborX::Point<DIM, Coordinate> const &minCorner,
                  ArborX::Point<DIM, Coordinate> const &maxCorner); // (2)

1) Default constructor. Constructs an invalid box, i.e., the coordinates of the minCorner are larger than the coordinates of the maxCorner.
2) Constructs a box from the given minimum (bottom left) corner and maximum (top right) corner.


Parameters
----------
``minCorner``: minimum (bottom left) corner\

``maxCorner``: maximum (top right) corner


Type requirements
^^^^^^^^^^^^^^^^^
* ``DIM`` must be an integer greater than zero
* ``Coordinate`` must be must be a type that is default constructible
