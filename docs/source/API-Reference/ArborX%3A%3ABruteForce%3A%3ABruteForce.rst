:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>` / :doc:`ArborX::BruteForce <ArborX%3A%3ABruteForce>`


``ArborX::BruteForce<MemorySpace, Value, IndexableGetter, BoundingVolume>::BruteForce``
=======================================================================================
.. code-block:: C++

    BruteForce() noexcept; // (1)

    template <typename ExecutionSpace, typename Values>
    BruteForce(ExecutionSpace const& space, Values const& values); // (2)

    template <typename ExecutionSpace, typename Values>
    BruteForce(ExecutionSpace const& space, Values const& values, IndexableGetter const& indexable_getter); // (3)

1) Default constructor.  Constructs an empty tree.
2) Constructs a brute force index from a given data source.
3) Constructs a brute force index from a given data source and a function tranforming a value into indexable.


Parameters
----------
* ``space``: the execution space.
* ``values``: values to store in the data structure.
* ``indexable_getter``: function object that extracts the geometry to index from a value.


Type requirements
^^^^^^^^^^^^^^^^^
* ``MemorySpace`` must be accessible from ``ExecutionSpace`` (i.e., ``Kokkos::SpaceAccessibility<ExecutionSpace, MemorySpace>::accessible`` must be ``true``).
* A specialization of :cpp:struct:`AccessTraits` must match ``Values`` as the template argument.
* The return type of :cpp:func:`AccessTraits::get` must decay to ``Value``.
* :doc:`spatial_indexes/IndexableGetter` must provide an ``operator()`` taking in a type that decays to ``Value`` and returning an indexable (geometric object).


Complexity
----------
*O(N)*, where *N* is the number of primitives passed to the constructor (``ArborX::AccessTraits<Values>::size(values)``).


Exceptions
----------
Memory allocation with Kokkos may throw.


Notes
-----


Example
-------


See also
--------
* :doc:`query <ArborX%3A%3ABruteForce%3A%3Aquery>`: search for all primitives that meet some predicates.
* :doc:`bounds <ArborX%3A%3ABruteForce%3A%3Abounds>`: returns the bounding volume that contains all leaves.
