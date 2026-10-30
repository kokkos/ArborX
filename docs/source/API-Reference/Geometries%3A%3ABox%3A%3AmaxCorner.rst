:orphan:

:doc:`ArborX </index>` / :doc:`Geometries <Geometries>` / :doc:`ArborX::Box <Geometries%3A%3ABox>`


``ArborX::Box<DIM, Coordinate>::maxCorner()``
=============================================
.. code-block:: C++

    KOKKOS_FUNCTION
    constexpr auto &maxCorner(); // (1)

    KOKKOS_FUNCTION
    constexpr auto const &maxCorner() const; // (2)

1) Returns a reference to the corner with maximal coordinates. This can be used to modify the corner.
2) Returns a constant reference to the corner with maximal coordinates.
