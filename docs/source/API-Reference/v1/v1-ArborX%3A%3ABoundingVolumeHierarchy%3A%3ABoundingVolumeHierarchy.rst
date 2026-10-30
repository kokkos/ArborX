:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes </API-Reference/SpatialIndexes>` / :doc:`ArborX::BVH <v1-ArborX%3A%3ABoundingVolumeHierarchy>`


``ArborX::BVH<MemorySpace>::BVH``
=================================
.. code-block:: C++

    BVH() noexcept; // (1)

    template <typename ExecutionSpace, typename Primitives>
    BVH(ExecutionSpace const& space, Primitives const& primitives); // (2)

1) Default constructor.  Constructs an empty tree.
2) Constructs a bounding volume hierarchy from the given data source.


Parameters
----------
* ``space``: the execution space.
* ``primitives``: geometrical objects one wishes to index.


Type requirements
^^^^^^^^^^^^^^^^^
* ``MemorySpace`` must be accessible from ``ExecutionSpace`` (i.e., ``Kokkos::SpaceAccessibility<ExecutionSpace, MemorySpace>::accessible`` must be ``true``).
* A specialization of :doc:`ArborX::AccessTraits <v1-ArborX%3A%3AAccessTraits>` must match ``Primitives`` as the first template argument and ``ArborX::PrimitivesTag`` as second argument.
* The return type of ``ArborX::AccessTraits<Primitives,ArborX::PrimitivesTag>::get()`` must decay either to ``ArborX::Point`` or ``ArborX::Box``.

ArborX provides specializations for Kokkos views but a user may specialize it for their types.


Complexity
----------
*O(N log N)*, where *N* is the number of primitives passed to the constructor (``ArborX::AccessTraits<Primitives,ArborX::PrimitivesTag>::size(primitives)``).


Exceptions
----------
Memory allocation with Kokkos may throw.


Notes
-----


Example
-------
.. code-block:: C++

    #include <ArborX_LinearBVH.hpp>

    #include <Kokkos_Core.hpp>

    #include <iostream>

    std::ostream &operator<<(std::ostream &os, ArborX::Point const &p)
    {
      os << '(' << p[0] << ',' << p[1] << ',' << p[2] << ')';
      return os;
    }

    int main(int argc, char *argv[])
    {
      Kokkos::ScopeGuard guard(argc, argv);

      Kokkos::View<ArborX::Point *> cloud("point_cloud", 1000);
      Kokkos::parallel_for(1000, KOKKOS_LAMBDA(int i) {
        cloud[i] = {{(float)i, (float)i, (float)i}};
      });

      using memory_space = decltype(cloud)::memory_space; // where to store the tree
      using execution_space = decltype(cloud)::execution_space; // where to execute code
      ArborX::BVH<memory_space> bvh{execution_space{}, cloud};

      auto const box = bvh.bounds();
      std::cout << box.minCorner() << " - " << box.maxCorner() << '\n';

      return 0;
    }

Output

.. code-block:: text

    (0,0,0) - (999,999,999)


See also
--------
* :doc:`query <v1-ArborX%3A%3ABoundingVolumeHierarchy%3A%3Aquery>`: search for all primitives that meet some predicates.
* :doc:`bounds <v1-ArborX%3A%3ABoundingVolumeHierarchy%3A%3Abounds>`: returns the bounding volume that contains all leaves.
