:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>` / :doc:`ArborX::BVH <ArborX%3A%3ABoundingVolumeHierarchy>`


``ArborX::BVH<MemorySpace, Value, IndexableGetter, BoundingVolume>::size()``
============================================================================
.. code-block:: C++

    KOKKOS_FUNCTION size_type size() const noexcept;

Returns the number of leaves in the tree.


Parameters
----------
(none)


Return value
------------
The number of leaves in the tree.


Complexity
----------
Constant.


See also
--------
* :doc:`empty <ArborX%3A%3ABoundingVolumeHierarchy%3A%3Aempty>`: checks whether the tree is empty.
* :doc:`bounds <ArborX%3A%3ABoundingVolumeHierarchy%3A%3Abounds>`: returns the bounding volume that contains all leaves.
