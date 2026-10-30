:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>` / :doc:`ArborX::DistributedTree <ArborX%3A%3ADistributedTree>`


``ArborX::DistributedTree<MemorySpace, Value, IndexableGetter, BoundingVolume>::empty()``
=========================================================================================
.. code-block:: C++

    KOKKOS_FUNCTION bool empty() const noexcept;

Checks if the global tree has no leaves on all ranks.


Parameters
----------
(none)


Return value
------------
``true`` if the global tree is empty, ``false`` otherwise.


Complexity
----------
Constant.


See also
--------
* :doc:`size <ArborX%3A%3ADistributedTree%3A%3Asize>`: returns the number of leaves on all ranks.
