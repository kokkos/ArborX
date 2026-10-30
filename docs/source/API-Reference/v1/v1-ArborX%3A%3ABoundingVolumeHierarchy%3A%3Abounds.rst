:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes </API-Reference/SpatialIndexes>` / :doc:`ArborX::BVH <v1-ArborX%3A%3ABoundingVolumeHierarchy>`


``ArborX::BVH<MemorySpace>::bounds()``
======================================
.. code-block:: C++

    KOKKOS_FUNCTION bounding_volume_type bounds() const noexcept;

Returns the smallest axis-aligned box able to contain all leaves stored in the tree.


Parameters
----------
(none)


Return value
------------
The smallest bounding volume that contains all leaves stored in the tree or an invalid bounding volume if the tree is empty.


Complexity
----------
Constant.


See also
--------
