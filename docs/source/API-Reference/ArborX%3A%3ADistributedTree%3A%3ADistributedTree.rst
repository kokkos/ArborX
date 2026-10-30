:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>` / :doc:`ArborX::DistributedTree <ArborX%3A%3ADistributedTree>`


``ArborX::DistributedTree<MemorySpace, Value, IndexableGetter, BoundingVolume>::DistributedTree``
=================================================================================================
.. code-block:: C++

    DistributedTree() noexcept; // (1)

    template <typename ExecutionSpace, typename Values>
    DistributedTree(MPI_Comm comm, ExecutionSpace const& space, Values const& values); // (2)

    template <typename ExecutionSpace, typename Values>
    DistributedTree(MPI_Comm comm, ExecutionSpace const& space, Values const& values, IndexableGetter const& indexable_getter); // (3)

1) Default constructor.  Constructs an empty tree.
2) Constructs a bounding volume hierarchy from a given data source.
3) Constructs a bounding volume hierarchy from a given data source and a function tranforming a value into indexable.


Parameters
----------
``comm`` : the MPI communicator.

``space``: execution space
``values``: values stored in the container
``indexable_getter``: a function object to transform a value into an indexable


Type requirements
^^^^^^^^^^^^^^^^^
* ``MemorySpace`` must be accessible from ``ExecutionSpace`` (i.e., ``Kokkos::SpaceAccessibility<ExecutionSpace, MemorySpace>::accessible`` must be ``true``).
* A specialization of :doc:`ArborX::AccessTraits <ArborX%3A%3AAccessTraits>` must match ``Values`` as the template argument.
* The return type of ``ArborX::AccessTraits<Values>::get()`` must decay to ``Value``.

  ArborX provides specializations for Kokkos views but a user may specialize it for their types.

* ``IndexableGetter`` must provide an ``operator()`` taking in a type that decays to ``Value`` and returning an indexable (geometric object).


Complexity
----------
*O(N log N)*, where *N* is the number of primitives passed to the constructor (``ArborX::AccessTraits<Values>::size(values)``).


Exceptions
----------
Memory allocation with Kokkos may throw.


Notes
-----
The constructor must be called by all MPI ranks related in the provided MPI communicator.


Example
-------
.. code-block:: C++

    #include <ArborX_DistributedTree.hpp>

    #include <Kokkos_Core.hpp>

    #include <iostream>

    int main(int argc, char *argv[])
    {
      MPI_Init(&argc, &argv);
      Kokkos::initialize(argc, argv);

      MPI_Comm const comm = MPI_COMM_WORLD;
      int comm_rank;
      MPI_Comm_rank(comm, &comm_rank);

      Kokkos::View<ArborX::Point<3> *> cloud("point_cloud", 10);
      Kokkos::parallel_for(10, KOKKOS_LAMBDA(int i) {
        cloud[i] = {{(float)i + 10 * comm_rank, (float)i, (float)i}};
      });

      using memory_space = decltype(cloud)::memory_space; // where to store the tree
      using execution_space = decltype(cloud)::execution_space; // where to execute code
      ArborX::DistributedTree dist_tree{comm, execution_space{}, cloud};

      if (comm_rank == 0)
      {
        auto const box = dist_tree.bounds();
        auto min = box.minCorner();
        auto max = box.maxCorner();
        std::cout << min[0] << ',' << min[1] << ',' << min[2] << " - "
                  << max[0] << ',' << max[1] << ',' << max[2] << '\n';
      }

      Kokkos::finalize();
      MPI_Finalize();

      return 0;
    }

Output for 4 MPI processes

.. code-block:: text

    (0,0,0) - (39,9,9)


See also
--------
* :doc:`query <ArborX%3A%3ADistributedTree%3A%3Aquery>`: search for all primitives that meet some predicates.
* :doc:`bounds <ArborX%3A%3ADistributedTree%3A%3Abounds>`: returns the bounding volume that contains all leaves.
