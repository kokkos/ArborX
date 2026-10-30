:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes </API-Reference/SpatialIndexes>`


``ArborX::BruteForce``
======================
Defined in header `<ArborX_BruteForce.hpp> <https://github.com/arborx/ArborX/tree/v1.7/src/ArborX_BruteForce.hpp>`_

.. code-block:: C++

    template <typename MemorySpace>
    class BruteForce;

The class template ``ArborX::BruteForce`` is a data structure that can be used to search for geometrical objects in space.  When performing queries, it checks all primitives against all predicates.


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
+----------------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
| :doc:`(constructor) <v1-ArborX%3A%3ABruteForce%3A%3ABruteForce>`           | constructs the data structure                                                                  |
+----------------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
| :doc:`size <v1-ArborX%3A%3ABruteForce%3A%3Asize>`                          | returns the number of elements stored in the data structure                                    |
+----------------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
| :doc:`empty <v1-ArborX%3A%3ABruteForce%3A%3Aempty>`                        | checks whether the data structure has no elements                                              |
+----------------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
| :doc:`bounds <v1-ArborX%3A%3ABruteForce%3A%3Abounds>`                      | returns an axis-aligned bounding box able to contain all elements stored in the data structure |
+----------------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
| :doc:`query <v1-ArborX%3A%3ABruteForce%3A%3Aquery>`                        | finds all elements that satisfy given predicates, e.g. intersecting with a box or a sphere     |
+----------------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
