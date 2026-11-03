:doc:`ArborX </index>` / :doc:`Predicates <Predicates>`


``ArborX::intersects``
======================
.. code-block:: C++

    template <typename Geometry>
    KOKKOS_INLINE_FUNCTION unspecified intersects(Geometry const& geometry);

Generate a spatial predicate to perform a search with in a spatial index' ``query()``.


Template parameter(s)
---------------------
* ``Geometry``: The geometry type, e.g. ``ArborX::Point``, ``ArborX::Box``, or ``ArborX::Sphere``.


Parameter(s)
------------
* ``geometry``: The geometry object.


Example
-------
.. code-block:: C++

    auto intersects_with_unit_sphere_pred = ArborX::intersects(ArborX::Sphere{{0.f, 0.f, 0.f}, 1.f});


Helper functions
----------------
ArborX provides a helper function to create ``intersects`` queries from a set of
user geometries (which could be provided through
:doc:`Access traits <AccessTraits>`):

.. code-block:: C++

    using Geometries = Kokkos::View<Geometry*>;
    Geometries geometries("geometries", 5);
    auto predicates = ArborX::Experimental::make_intersects(geometries);

This is equivalent to creating a set of predicates ``intersects(AccessTraits<Geometries>::get(i))``.

A second helper function creates a set of ``intersects`` queries for spheres of
the given radius centered at the input points:

.. code-block:: C++

    using Points = Kokkos::View<ArborX::Point<3>*>;
    Points points("points", 5);
    float radius = 1.f;
    auto predicates_with_radius = ArborX::Experimental::make_intersects(points, radius);

This is equivalent to creating a set of predicates
``intersects(ArborX::Sphere(AccessTraits<Points>::get(points, i), radius))``.
