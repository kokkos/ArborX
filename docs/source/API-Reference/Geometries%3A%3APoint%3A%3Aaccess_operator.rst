:orphan:

:doc:`ArborX </index>` / :doc:`Geometries <Geometries>` / :doc:`ArborX::Point <Geometries%3A%3APoint>`


``ArborX::Point<DIM, Coordinate>::operator[]``
==============================================
.. code-block:: C++

    KOKKOS_FUNCTION
    constexpr auto &operator[](unsigned int i); // (1)

    KOKKOS_FUNCTION
    constexpr auto const &operator[](unsigned int i) const; // (2)

1) Returns a reference to the value in the i-th dimension. This can be used to modify the coordinates of the point.
2) Returns a constant reference to the value in the i-th dimension.
