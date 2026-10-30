:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>`


``ArborX::BruteForce``
======================
.. note::

   For ArborX 1.x, see
   :doc:`ArborX::BruteForce (v1) </API-Reference/v1/v1-ArborX%3A%3ABruteForce>`.

Defined in header `<ArborX_BruteForce.hpp> <https://github.com/arborx/ArborX/blob/master/src/spatial/ArborX_BruteForce.hpp>`_

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
+----------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
| :doc:`(constructor) <ArborX%3A%3ABruteForce%3A%3ABruteForce>`        | constructs the data structure                                                                  |
+----------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
| :doc:`size <ArborX%3A%3ABruteForce%3A%3Asize>`                       | returns the number of elements stored in the data structure                                    |
+----------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
| :doc:`empty <ArborX%3A%3ABruteForce%3A%3Aempty>`                     | checks whether the data structure has no elements                                              |
+----------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
| :doc:`bounds <ArborX%3A%3ABruteForce%3A%3Abounds>`                   | returns an axis-aligned bounding box able to contain all elements stored in the data structure |
+----------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
| :doc:`query <ArborX%3A%3ABruteForce%3A%3Aquery>`                     | finds all elements that satisfy given predicates, e.g. intersecting with a box or a sphere     |
+----------------------------------------------------------------------+------------------------------------------------------------------------------------------------+
