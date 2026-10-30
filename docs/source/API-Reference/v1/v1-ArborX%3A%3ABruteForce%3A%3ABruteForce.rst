:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes </API-Reference/SpatialIndexes>` / :doc:`ArborX::BruteForce <v1-ArborX%3A%3ABruteForce>`


``ArborX::BruteForce<MemorySpace>::BruteForce``
===============================================
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
* A specialization of :doc:`ArborX::AccessTraits <v1-ArborX%3A%3AAccessTraits>` must match ``Primitives`` as the first template argument and ``ArborX::PrimitivesTag`` as second argument.
* The return type of ``ArborX::AccessTraits<Primitives,ArborX::PrimitivesTag>::get()`` must decay either to ``ArborX::Point`` or ``ArborX::Box``.

ArborX provides specializations for Kokkos views but a user may specialize it for their types.


Complexity
----------
*O(N)*, where *N* is the number of primitives passed to the constructor (``ArborX::AccessTraits<Primitives,ArborX::PrimitivesTag>::size(primitives)``).


Exceptions
----------
Memory allocation with Kokkos may throw.


Notes
-----


Example
-------


See also
--------
* :doc:`query <v1-ArborX%3A%3ABoundingVolumeHierarchy%3A%3Aquery>`: search for all primitives that meet some predicates.
* :doc:`bounds <v1-ArborX%3A%3ABoundingVolumeHierarchy%3A%3Abounds>`: returns the box that contains all leaves.
