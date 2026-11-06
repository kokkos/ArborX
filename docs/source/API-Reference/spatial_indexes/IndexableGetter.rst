``IndexableGetter``
===================
Defined in header `<ArborX_IndexableGetter.hpp> <https://github.com/arborx/ArborX/blob/master/src/spatial/detail/ArborX_IndexableGetter.hpp>`_

A spatial index stores *values* and searches them by the geometry they occupy
in space, called their *indexable*.
The ``IndexableGetter`` template argument of a spatial index (e.g.,
:cpp:class:`BoundingVolumeHierarchy`) is the function object that extracts the
indexable from a value.

An ``IndexableGetter`` must

* provide a ``const`` ``operator()`` taking in a type that decays to the
  ``Value`` of the spatial index and returning an indexable, i.e., a geometry
  such as a :doc:`Point </API-Reference/Geometries>`, and
* be callable from the device, which means its call operator needs to be marked
  with the ``KOKKOS_FUNCTION`` macro.

When the values are geometries, or wrap a geometry in an
``ArborX::PairValueIndex``, :cpp:struct:`Experimental::DefaultIndexableGetter`
is all that is needed and is the default.
A custom one is required when the geometry is not stored in the value itself.


``DefaultIndexableGetter``
--------------------------
.. cpp:struct:: Experimental::DefaultIndexableGetter

   The default ``IndexableGetter``.
   It uses the value itself as the indexable, or the geometry it wraps if the
   value is an ``ArborX::PairValueIndex``.

   .. cpp:function:: KOKKOS_FUNCTION DefaultIndexableGetter() = default

   .. cpp:function:: template <typename Geometry> \
                     KOKKOS_FUNCTION auto const &operator()(Geometry const &geometry) const

      :return: ``geometry``.

   .. cpp:function:: template <typename Geometry, typename Index> \
                     KOKKOS_FUNCTION Geometry const &operator()(PairValueIndex<Geometry, Index> const &pair) const

      :return: ``pair.value``, the geometry wrapped by ``pair``.

   A geometry that is not ``const`` (an rvalue or a non-``const`` lvalue) is
   returned by value, and so is the geometry of an rvalue
   ``ArborX::PairValueIndex``, instead of a reference.


Example
-------
The values below are the identifiers of the objects, and the geometry of each
object lives in a separate view.
The default getter cannot be used since an identifier is not a geometry, so we
provide one that looks up the point of the object.

.. code-block:: C++

    template <typename MemorySpace>
    struct PointOfObject
    {
      Kokkos::View<ArborX::Point<3> *, MemorySpace> points;

      KOKKOS_FUNCTION ArborX::Point<3> operator()(int id) const
      {
        return points(id);
      }
    };

    // ids is a Kokkos::View<int *, MemorySpace>
    ArborX::BoundingVolumeHierarchy bvh(space, ids,
                                        PointOfObject<MemorySpace>{points});


See also
--------
* :doc:`BoundingVolumeHierarchy`
* :doc:`/API-Reference/AccessTraits`
