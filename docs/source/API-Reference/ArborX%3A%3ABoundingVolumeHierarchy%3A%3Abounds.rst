:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>` / :doc:`ArborX::BVH <ArborX%3A%3ABoundingVolumeHierarchy>`


``ArborX::BVH<MemorySpace, Value, IndexableGetter, BoundingVolume>::bounds()``
==============================================================================
.. code-block:: C++

    KOKKOS_FUNCTION bounding_volume_type bounds() const noexcept;

Returns a bounding volume of type ``BoundingVolume`` containing all leaves stored in the tree.


Parameters
----------
(none)


Return value
------------
A bounding volume containing all leaves stored in the tree or an invalid bounding volume if the tree is empty.


Complexity
----------
Constant.


See also
--------
