:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>` / :doc:`ArborX::BVH <ArborX%3A%3ABoundingVolumeHierarchy>`


``ArborX::BVH<MemorySpace, Value, IndexableGetter, BoundingVolume>::empty()``
=============================================================================
.. code-block:: C++

    KOKKOS_FUNCTION bool empty() const noexcept;

Checks if the tree has no leaves.


Parameters
----------
(none)


Return value
------------
``true`` if the tree is empty, ``false`` otherwise.


Complexity
----------
Constant.


See also
--------
* :doc:`size <ArborX%3A%3ABoundingVolumeHierarchy%3A%3Asize>`: returns the number of leaves.
