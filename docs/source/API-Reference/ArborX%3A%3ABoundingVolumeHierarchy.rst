:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>`


``ArborX::BVH``
===============
.. note::

   For ArborX 1.x, see
   :doc:`ArborX::BoundingVolumeHierarchy (v1) </API-Reference/v1/v1-ArborX%3A%3ABoundingVolumeHierarchy>`.

Defined in header `<ArborX_LinearBVH.hpp> <https://github.com/arborx/ArborX/blob/master/src/spatial/ArborX_LinearBVH.hpp>`_

.. code-block:: C++

    template <typename MemorySpace, typename Value, typename IndexableGetter, typename BoundingVolume>
    class BoundingVolumeHierarchy;

The class template ``ArborX::BoundingVolumeHierarchy`` is a tree data structure that can be used to accelerate the search for geometrical objects in space.

..
   ArborX defines the ``ArborX::BVH`` alias template for convenience.

   .. code-block:: C++

       template <typename MemorySpace, typename Value, typename IndexableGetter, typename BoundingVolume>
       using BVH = BoundingVolumeHierarchy<MemorySpace, Value, IndexableGetter, BoundingVolume>;


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
   * - ``value_type``
     - ``Value``
   * - ``size_type``
     - ``MemorySpace::size_type``
   * - ``bounding_volume_type``
     - ``BoundingVolume``
   * - ``indexable_getter_type``
     - ``IndexableGetter``


Member functions
----------------
+------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`(constructor) <ArborX%3A%3ABoundingVolumeHierarchy%3A%3ABoundingVolumeHierarchy>`        | constructs the tree data structure                                                                             |
+------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`size <ArborX%3A%3ABoundingVolumeHierarchy%3A%3Asize>`                                    | returns the number of leaves stored in the tree                                                                |
+------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`empty <ArborX%3A%3ABoundingVolumeHierarchy%3A%3Aempty>`                                  | checks whether the tree has no leaves                                                                          |
+------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`bounds <ArborX%3A%3ABoundingVolumeHierarchy%3A%3Abounds>`                                | returns a bounding volume able to contain all leaves stored in the tree                                        |
+------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`query <ArborX%3A%3ABoundingVolumeHierarchy%3A%3Aquery>`                                  | finds all leaves that satisfy given predicates, e.g. nearest to a point or intersecting with a box or a sphere |
+------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
