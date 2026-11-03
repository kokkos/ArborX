``intersects``
==============
Defined in header `<ArborX_Predicates.hpp> <https://github.com/arborx/ArborX/blob/master/src/spatial/detail/ArborX_Predicates.hpp>`_

.. cpp:function:: template <typename Geometry> \
                  KOKKOS_INLINE_FUNCTION auto intersects(Geometry const &geometry)

   Generate a spatial predicate to perform a search with in a spatial index'
   ``query()``.

   :tparam Geometry: The geometry type, e.g.,
      :doc:`ArborX::Point </API-Reference/Geometries%3A%3APoint>`,
      :doc:`ArborX::Box </API-Reference/Geometries%3A%3ABox>`, or
      ``ArborX::Sphere``.
      See :doc:`Geometries </API-Reference/Geometries>` for the geometries
      provided by ArborX.
   :param geometry: The geometry object.
   :return: A predicate satisfied by the values stored in the index whose
      indexable intersects ``geometry``.
      The type of the returned object is unspecified.

Example
-------
.. code-block:: C++

   auto intersects_with_unit_sphere_pred =
       ArborX::intersects(ArborX::Sphere{ArborX::Point{0.f, 0.f, 0.f}, 1.f});

The returned predicate can also be applied to a geometry.
It returns ``true`` if that geometry intersects ``geometry``, and ``false``
otherwise.

.. code-block:: C++

   auto pred = ArborX::intersects(
       ArborX::Box{ArborX::Point{0.f, 0.f}, ArborX::Point{1.f, 1.f}});
   bool hit  = pred(ArborX::Point{.5f, .5f});  // true
   bool miss = pred(ArborX::Point{2.f, 0.f});  // false


Helper functions
----------------
The helper functions create a set of predicates from a set of geometries
rather than from a single one.
The geometries are provided through :cpp:struct:`AccessTraits`, for instance as
a ``Kokkos::View``.
The result is meant to be passed directly as the ``predicates`` argument of
``query()``.

.. cpp:function:: template <typename Primitives> \
                  auto Experimental::make_intersects(Primitives const &primitives)

   Create a set of spatial predicates, one per geometry.

   This is equivalent to creating the set of predicates
   ``intersects(AccessTraits<Primitives>::get(primitives, i))``
   for ``i`` in ``[0, AccessTraits<Primitives>::size(primitives))``.

   .. code-block:: C++

      using Geometries = Kokkos::View<Geometry*>;
      Geometries geometries("geometries", 5);
      auto predicates = ArborX::Experimental::make_intersects(geometries);

.. cpp:function:: template <typename Primitives, typename Coordinate> \
                  auto Experimental::make_intersects(Primitives const &primitives, \
                                                     Coordinate r)

   Create a set of within-radius spatial predicates, one per point.

   This is equivalent to creating the set of predicates
   ``intersects(ArborX::Sphere(AccessTraits<Primitives>::get(primitives, i), r))``.

   :param primitives: A set of points.
   :param r: The search radius. Must be strictly positive.

   .. code-block:: C++

      using Points = Kokkos::View<ArborX::Point<3>*>;
      Points points("points", 5);
      float radius = 1.f;
      auto predicates_with_radius =
          ArborX::Experimental::make_intersects(points, radius);


See also
--------
* :doc:`Predicates </API-Reference/Predicates>`
* :cpp:func:`nearest`
* :doc:`Callbacks </API-Reference/Callbacks>`
* :cpp:func:`BoundingVolumeHierarchy::query`
