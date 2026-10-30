:orphan:

:doc:`ArborX </index>` / :doc:`Spatial indexes <SpatialIndexes>` / :doc:`ArborX::BVH <ArborX%3A%3ABoundingVolumeHierarchy>`


``ArborX::BVH<MemorySpace, Value, IndexableGetter, BoundingVolume>::BVH``
=========================================================================
.. code-block:: C++

    BVH() noexcept; // (1)

    template <typename ExecutionSpace, typename Values>
    BVH(ExecutionSpace const& space, Values const& values); // (2)

    template <typename ExecutionSpace, typename Values>
    BVH(ExecutionSpace const& space, Values const& values, IndexableGetter const& indexable_getter); // (3)

1) Default constructor.  Constructs an empty tree.
2) Constructs a bounding volume hierarchy from a given data source.
3) Constructs a bounding volume hierarchy from a given data source and a function tranforming a value into indexable.


Parameters
----------
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

      Kokkos::View<ArborX::Point<3> *> cloud("point_cloud", 1000);
      Kokkos::parallel_for(1000, KOKKOS_LAMBDA(int i) {
        cloud[i] = {(float)i, (float)i, (float)i};
      });

      using memory_space = decltype(cloud)::memory_space; // where to store the tree
      using execution_space = decltype(cloud)::execution_space; // where to execute code
      ArborX::BVH bvh{execution_space{}, cloud};

      auto const box = bvh.bounds();
      std::cout << box.minCorner() << " - " << box.maxCorner() << '\n';

      return 0;
    }

Output

.. code-block:: text

    (0,0,0) - (999,999,999)


See also
--------
* :doc:`query <ArborX%3A%3ABoundingVolumeHierarchy%3A%3Aquery>`: search for all primitives that meet some predicates.
* :doc:`bounds <ArborX%3A%3ABoundingVolumeHierarchy%3A%3Abounds>`: returns the bounding volume that contains all leaves.
