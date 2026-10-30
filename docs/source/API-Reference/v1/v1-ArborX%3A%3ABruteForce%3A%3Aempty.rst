:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes </API-Reference/SpatialIndexes>` / :doc:`ArborX::BruteForce <v1-ArborX%3A%3ABruteForce>`


``ArborX::BruteForce<MemorySpace>::empty()``
============================================
.. code-block:: C++

    KOKKOS_FUNCTION bool empty() const noexcept;

Checks if the data structure has no elements.


Parameters
----------
(none)


Return value
------------
``true`` if the data structure is empty, ``false`` otherwise.


Complexity
----------
Constant.


See also
--------
* :doc:`size <v1-ArborX%3A%3ABruteForce%3A%3Asize>`: returns the number of elements.
