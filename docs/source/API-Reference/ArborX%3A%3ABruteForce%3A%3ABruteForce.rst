:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>` / :doc:`ArborX::BruteForce <ArborX%3A%3ABruteForce>`


``ArborX::BruteForce<MemorySpace, Value, IndexableGetter, BoundingVolume>::BruteForce``
=======================================================================================
.. code-block:: C++

    BruteForce() noexcept; // (1)

    template <typename ExecutionSpace, typename Primitives>
    BruteForce(ExecutionSpace const& space, Primitives const& primitives); // (2)

1) Default constructor.  Constructs an empty data structure.
2) Constructs a bounding volume hierarchy from the given data source.


Parameters
----------
* ``space``: the execution space.
* ``primitives``: geometrical objects one wishes to index.


Type requirements
^^^^^^^^^^^^^^^^^
* ``MemorySpace`` must be accessible from ``ExecutionSpace``. (``Kokkos::SpaceAccessibility<ExecutionSpace, MemorySpace>::accessible`` must be ``true``.)
* A specialization of :doc:`ArborX::AccessTraits <ArborX%3A%3AAccessTraits>` must match ``Primitives`` as the first template argument.
* The return type of ``ArborX::AccessTraits<Primitives>::get()`` must decay either to ``ArborX::Point`` or ``ArborX::Box``.

ArborX provides specializations for Kokkos views but a user may specialize it for their types.


Complexity
----------
*O(N)*, where *N* is the number of primitives passed to the constructor (``ArborX::AccessTraits<Primitives>::size(primitives)``).


Exceptions
----------
Memory allocation with Kokkos may throw.


Notes
-----


Example
-------


See also
--------
* :doc:`query <ArborX%3A%3ABoundingVolumeHierarchy%3A%3Aquery>`: search for all primitives that meet some predicates.
* :doc:`bounds <ArborX%3A%3ABoundingVolumeHierarchy%3A%3Abounds>`: returns the box that contains all leaves.
