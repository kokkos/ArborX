:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>` / :doc:`ArborX::BruteForce <ArborX%3A%3ABruteForce>`


``ArborX::BruteForce<MemorySpace, Value, IndexableGetter, BoundingVolume>::bounds()``
=====================================================================================
.. code-block:: C++

    KOKKOS_FUNCTION bounding_volume_type bounds() const noexcept;

Returns a bounding volume of type ``BoundingVolume`` that contains all indexed values.


Parameters
----------
(none)


Return value
------------
A bounding volume that contains all indexed values, or an invalid bounding volume if the data structure is empty.


Complexity
----------
Constant.


See also
--------
