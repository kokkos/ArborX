:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>`


``ArborX::DistributedTree``
===========================
Defined in header `<ArborX_DistributedTree.hpp> <https://github.com/arborx/ArborX/blob/master/src/distributed/ArborX_DistributedTree.hpp>`_

.. code-block:: C++

    template <typename MemorySpace, typename Value, typename IndexableGetter, typename BoundingVolume>
    class DistributedTree;

The class template ``ArborX::DistributedTree`` is a tree data structure that can be used to accelerate the search for geometrical objects in space distributed across multiple MPI processes.


Template parameter
------------------
* ``MemorySpace``: valid Kokkos memory space
* ``Value``: type of objects stored in the container
* ``IndexableGetter``: function extracting ``Indexable`` from ``Value``
* ``BoundingVolume``: bounding volume used for internal nodes


Member types
------------
.. list-table::
   :align: left
   :class: member-types-table
   :header-rows: 1

   * - Member type
     - Definition
   * - ``memory_space``
     - ``MemorySpace``
   * - ``size_type``
     - ``typename BVH<MemorySpace>::size_type``
   * - ``value_type``
     - ``Value``
   * - ``bounding_volume_type``
     - ``typename BVH<MemorySpace>::bounding_volume_type``
   * - ``indexable_getter_type``
     - ``IndexableGetter``


Member functions
----------------
+--------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`(constructor) <ArborX%3A%3ADistributedTree%3A%3ADistributedTree>`        | constructs the tree data structure                                                                             |
+--------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`size <ArborX%3A%3ADistributedTree%3A%3Asize>`                            | returns the global number of objects stored in the tree                                                        |
+--------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`empty <ArborX%3A%3ADistributedTree%3A%3Aempty>`                          | checks whether the tree is empty on all ranks                                                                  |
+--------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`bounds <ArborX%3A%3ADistributedTree%3A%3Abounds>`                        | returns a bounding volume able to contain all leaves stored in the tree                                        |
+--------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`query <ArborX%3A%3ADistributedTree%3A%3Aquery>`                          | finds all leaves that satisfy given predicates, e.g. nearest to a point or intersecting with a box or a sphere |
+--------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
