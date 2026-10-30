:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes </API-Reference/SpatialIndexes>` / :doc:`ArborX::BVH <v1-ArborX%3A%3ABoundingVolumeHierarchy>`


``ArborX::BVH<MemorySpace>::empty()``
=====================================
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
* :doc:`size <v1-ArborX%3A%3ABoundingVolumeHierarchy%3A%3Asize>`: returns the number of leaves.
