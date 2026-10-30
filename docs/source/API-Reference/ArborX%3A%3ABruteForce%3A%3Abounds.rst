:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>` / :doc:`ArborX::BruteForce <ArborX%3A%3ABruteForce>`


``ArborX::BruteForce<MemorySpace>::bounds()``
=============================================
.. code-block:: C++

    KOKKOS_FUNCTION Box bounds() const noexcept;

Returns the smallest axis-aligned box able to contain all elements stored in the data structure.


Parameters
----------
(none)


Return value
------------
The smallest axis-aligned bounding box that contains all elements stored in the data structure or an invalid bounding volume if the data structure is empty.


Complexity
----------
Constant.


See also
--------
