:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes </API-Reference/SpatialIndexes>`


``ArborX::BVH``
===============
Defined in header `<ArborX_LinearBVH.hpp> <https://github.com/arborx/ArborX/tree/v1.7/src/ArborX_LinearBVH.hpp>`_

.. code-block:: C++

    template <typename MemorySpace>
    class BoundingVolumeHierarchy;

The class template ``ArborX::BoundingVolumeHierarchy`` is a tree data structure that can be used to accelerate the search for geometrical objects in space.

ArborX defines the ``ArborX::BVH`` alias template for convenience.

.. code-block:: C++

    template <typename MemorySpace>
    using BVH = BoundingVolumeHierarchy<MemorySpace>;


Template parameter
------------------
* ``MemorySpace``: A valid Kokkos memory space.


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
     - ``MemorySpace::size_type``
   * - ``bounding_volume_type``
     - ``ArborX::Box``


Member functions
----------------
+------------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`(constructor) <v1-ArborX%3A%3ABoundingVolumeHierarchy%3A%3ABoundingVolumeHierarchy>`           | constructs the tree data structure                                                                             |
+------------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`size <v1-ArborX%3A%3ABoundingVolumeHierarchy%3A%3Asize>`                                       | returns the number of leaves stored in the tree                                                                |
+------------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`empty <v1-ArborX%3A%3ABoundingVolumeHierarchy%3A%3Aempty>`                                     | checks whether the tree has no leaves                                                                          |
+------------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`bounds <v1-ArborX%3A%3ABoundingVolumeHierarchy%3A%3Abounds>`                                   | returns a bounding volume able to contain all leaves stored in the tree                                        |
+------------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
| :doc:`query <v1-ArborX%3A%3ABoundingVolumeHierarchy%3A%3Aquery>`                                     | finds all leaves that satisfy given predicates, e.g. nearest to a point or intersecting with a box or a sphere |
+------------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------------------+
