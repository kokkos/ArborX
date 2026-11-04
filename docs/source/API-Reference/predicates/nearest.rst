``nearest``
===========
Defined in header `<ArborX_Predicates.hpp> <https://github.com/arborx/ArborX/blob/master/src/spatial/detail/ArborX_Predicates.hpp>`_

.. cpp:function:: template <typename Geometry> \
                  KOKKOS_INLINE_FUNCTION auto nearest(Geometry const &geometry, int k = 1)

   Generate a nearest predicate to perform a *k*-nearest neighbors search with
   in a spatial index' ``query()``.

   :tparam Geometry: The geometry type, e.g.,
      :doc:`ArborX::Point </API-Reference/Geometries%3A%3APoint>`,
      :doc:`ArborX::Box </API-Reference/Geometries%3A%3ABox>`, or
      ``ArborX::Sphere``.
      See :doc:`Geometries </API-Reference/Geometries>` for the geometries
      provided by ArborX.
   :param geometry: The geometry object from which distance is calculated.
   :param k: The number of values to search for.
   :return: A predicate satisfied by the ``k`` values stored in the index whose
      indexable is the closest to ``geometry``.
      If the index stores fewer than ``k`` values, all of them are found.
      The type of the returned object is unspecified.

Example
-------
.. code-block:: C++

   auto nearest_five_to_origin_pred =
       ArborX::nearest(ArborX::Point{0.f, 0.f, 0.f}, 5);


Helper functions
----------------
The helper function creates a set of predicates from a set of geometries
rather than from a single one.
The geometries are provided through :cpp:struct:`AccessTraits`, for instance as
a ``Kokkos::View``.
The result is meant to be passed directly as the ``predicates`` argument of
``query()``.

.. cpp:function:: template <typename Primitives> \
                  auto Experimental::make_nearest(Primitives const &primitives, int k)

   Create a set of nearest predicates, one per geometry.

   This is equivalent to creating the set of predicates
   ``nearest(AccessTraits<Primitives>::get(primitives, i), k)``.

   :param primitives: A set of geometries.
   :param k: The number of values to search for, for each geometry.

   .. code-block:: C++

      using Geometries = Kokkos::View<Geometry*>;
      Geometries geometries("geometries", 5);
      int k = 3;
      auto predicates = ArborX::Experimental::make_nearest(geometries, k);


See also
--------
* :doc:`Predicates </API-Reference/Predicates>`
* :cpp:func:`intersects`
* :doc:`Callbacks </API-Reference/Callbacks>`
* :doc:`ArborX::BVH::query() </API-Reference/ArborX%3A%3ABoundingVolumeHierarchy%3A%3Aquery>`
