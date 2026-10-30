:doc:`ArborX </index>` / :doc:`Predicates <Predicates>`


``ArborX::nearest``
===================
.. code-block:: C++

    template <typename Geometry>
    KOKKOS_FUNCTION unspecified nearest(Geometry const& geometry, std::size_t k = 1) noexcept;

Generate a nearest predicate to perform a $k$-nearest neighbors search with in a spatial index' ``query()``.


Template parameter(s)
---------------------
* ``Geometry``: The geometry type, e.g. ``ArborX::Point``, ``ArborX::Box``, or ``ArborX::Sphere``.


Parameter(s)
------------
* ``geometry``: The geometry object from which distance is calculated.
* ``k``: The number of primitives to search for.


Example
-------
.. code-block:: C++

    auto nearest_five_to_origin_pred = ArborX::nearest(ArborX::Point{0.f, 0.f, 0.f}, 5);


Helper functions
----------------
ArborX provides a helper function to create ``nearest`` queries from a set of
user geometries (which could be provided through
:doc:`Access traits <ArborX%3A%3AAccessTraits>`):

.. code-block:: C++

    using Geometries = Kokkos::View<Geometry*>;
    Geometries geometries("geometries", 5);
    int k = 3;
    auto predicates = ArborX::Experimental::make_nearest(geometries, k);

This is equivalent to creating a set of predicates ``nearest(AccessTraits<Geometries>::get(i), k)``.
