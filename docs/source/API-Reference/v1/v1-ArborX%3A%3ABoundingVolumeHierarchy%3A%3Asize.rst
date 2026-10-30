:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes </API-Reference/SpatialIndexes>` / :doc:`ArborX::BVH <v1-ArborX%3A%3ABoundingVolumeHierarchy>`


``ArborX::BVH<MemorySpace>::size()``
====================================
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
* :doc:`empty <v1-ArborX%3A%3ABoundingVolumeHierarchy%3A%3Aempty>`: checks whether the tree is empty.
* :doc:`bounds <v1-ArborX%3A%3ABoundingVolumeHierarchy%3A%3Abounds>`: returns the bounding volume that contains all leaves.
