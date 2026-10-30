:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>` / :doc:`ArborX::DistributedTree <ArborX%3A%3ADistributedTree>`


``ArborX::DistributedTree<MemorySpace, Value, IndexableGetter, BoundingVolume>::size()``
========================================================================================
.. code-block:: C++

    KOKKOS_FUNCTION size_type size() const noexcept;

Returns the global number of leaves in the tree.


Parameters
----------
(none)


Return value
------------
The global number of leaves in the tree.


Complexity
----------
Constant.


See also
--------
* :doc:`empty <ArborX%3A%3ADistributedTree%3A%3Aempty>`: checks whether the tree is empty.
* :doc:`bounds <ArborX%3A%3ADistributedTree%3A%3Abounds>`: returns a bounding volume that contains all leaves.
