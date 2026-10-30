:doc:`ArborX </index>`


Geometries
==========


``ArborX::GeometryTraits``
--------------------------
Defined in header `<ArborX_GeometryTraits.hpp> <https://github.com/arborx/ArborX/blob/master/src/geometry/ArborX_GeometryTraits.hpp>`_

The struct templates specialized in the ``ArborX::GeometryTraits`` namespace provide ArborX information about geometry.

.. code-block:: C++

    template <typename Geometry>
    struct ArborX::GeometryTraits::dimension<Geometry>
    {
      static constexpr int value = ...;
    };

Specialization of ``dimension`` describes geometry's dimension.

.. code-block:: C++

    template <typename Geometry>
    struct ArborX::GeometryTraits::coordinate_type<Geometry>
    {
      using type = ...;
    };

Specialization of ``coordinate_type`` describes the precision of the stored data.

.. code-block:: C++

    template <typename Geometry>
    struct ArborX::GeometryTraits::tag<Geometry>
    {
      using type = ...;
    };

Specialization of ``tag`` describes the type of the geometry.

.. note::

   For a user provided data, ``dimension`` and ``coordinate_type`` are required
   specializations. If a ``tag`` specialization is provided and is one of the
   ArborX known tags, the algorithms implemented in ArborX will be used for
   calculations.


Provided Geometries
-------------------
Currently, ArborX provides the following geometries:

- :doc:`Point <Geometries%3A%3APoint>`
- :doc:`Box <Geometries%3A%3ABox>` (axis-aligned)
- ``Sphere``
- ``Triangle``
- ``KDOP`` [EXPERIMENTAL]
- ``Ellipsoid`` (EXPERIMENTAL]
- ``Ray`` [EXPERIMENTAL]
- ``Segment`` [EXPERIMENTAL]
- ``Tetrahedron`` [EXPERIMENTAL]

.. toctree::
   :hidden:
   :maxdepth: 1
   :titlesonly:

   Geometries%3A%3APoint
   Geometries%3A%3ABox
