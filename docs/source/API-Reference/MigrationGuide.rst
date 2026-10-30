:doc:`ArborX </index>`


APIv1 to APIv2 migration guide
==============================
APIv2 (used in ArborX 2.x) extensively changed the spatial index API, breaking
backwards compatibility. This guide provides motivation behind the change and
general tips for migrating an existing APIv1 (ArborX 1.x) code to APIv2.


Motivation
----------
APIv1 had multiple severe limitations:

- Dimensionality was hardcoded to 3D

  APIv1 only supported 3D objects. 2D objects were treated as 3D through
  setting $z = 0$, at the cost of extra memory storage and slower traversal.

- User-provided data was restricted to points and boxes

  Spatial indexes lacked the capability of storing general user data, including
  non-trivial geometries (e.g., triangles, tetrahedrons, etc) and associated
  metadata (e.g., global index corresponding to a geometry).

- Only coarse search was supported

  As APIv1 was only able to store user data as points and boxes, the queries
  were only able to perform coarse search.

  For spatial queries, that meant that a user had to provide a callback where
  the fine search (intersection with user geometries) was performed.

  For nearest queries, this meant that ArborX only considered the distances to
  stored boxes, and not to the user data. This meant that for user geometries
  other than points and boxes, the results produced by ArborX could be wrong.

- Callbacks were limited

  Signatures of callbacks only took integer as the second argument, requiring
  user to store more data inside the callback.


Migration tips
--------------


Geometries
~~~~~~~~~~
In APIv1, ``ArborX::Point`` and ``ArborX::Box`` were hardcoded to single-precision

3D data. In contrast, all geometries in APIv2 are templated on both the
dimension and the stored type:

.. code-block:: text

    template<int DIM, typename Coordinate = float>
    struct Geometry;

Some geometries, for example KDOP, may have additional template arguments.


``AccessTraits``
~~~~~~~~~~~~~~~~
APIv2 removed the second template argument in ``AccessTraits``, which in APIv1
had to be ``ArborX::PrimitivesTag`` or ``ArborX::PredicatesTag``.

In most situations, simply removing this argument in the user code is
sufficient. One exception is when the code provided ``AccessTraits``
specialization for the same data structure for both ``PrimitivesTag`` and
``PredicatesTag``. In this case, the simplest approach would be to introduce user
space tags and template user data structure on those.

For example,

.. code-block:: c++

    struct PrimitivesTag{};
    struct PredicatesTag{};

    template<typename Tag, typename... Args>
    struct UserData {...};

    template<typename... Args>
    struct ArborX::AccessTraits<UserData<PrimitivesTag, Args...>> {...};

    template<typename... Args>
    struct ArborX::AccessTraits<UserData<PredicatesTag, Args...>> {...};


APIv2 in APIv1 mode
~~~~~~~~~~~~~~~~~~~
APIv1 used ``int`` as the default returned value or as the second argument to the
callback. To reproduce this behavior, construct an index with attached indexes
and use a callback to extract the index:

.. code-block:: c++

    struct ExtractIndex
    {
      template <typename Query, typename Value, typename Output>
      KOKKOS_FUNCTION void operator()(Query const &, Value const &value,
                                      Output const &out) const
      {
        out(value.index);
      }
    };

    Primitives primitives;
    using Value = ...; // return type of ArborX::AccessTraits<Primitives>::get()

    // The template arguments could be omitted (to make use of deduction guides)
    BoundingVolumeHierarchy<MemorySpace, ArborX::PairValueIndex<Value,int>>
      bvh(space, ArborX::Experimental::attach_indices(primitives));
    Kokkos::View<int*> offsets("offsets", 0);
    Kokkos::View<int*> indices("indices", 0);
    bvh.query(space, predicates, ExtractIndex{}, indices, offsets);


ARBORX_VERSION
--------------
If one desires to be able to build code against both versions of ArborX,

``ARBORX_VERSION`` macro can be used to distinguish whether the code is built
against APIv1 (``ARBORX_VERSION < 20000``) or APIv2 (``ARBORX_VERSION >= 20000``).
