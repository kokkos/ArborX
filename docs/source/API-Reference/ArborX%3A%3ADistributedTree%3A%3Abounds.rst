:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>` / :doc:`ArborX::DistributedTree <ArborX%3A%3ADistributedTree>`


``ArborX::DistributedTree<MemorySpace, Value, IndexableGetter, BoundingVolume>::bounds()``
==========================================================================================
.. code-block:: C++

    KOKKOS_FUNCTION bounding_volume_type bounds() const noexcept;

Returns a bounding volume of type ``BoundingVolume`` able to contain all leaves stored in the tree globally.


Parameters
----------
(none)


Return value
------------
A bounding volume containing all leaves stored in the tree globally or an invalid bounding volume if the tree is empty.


Complexity
----------
Constant.


See also
--------
