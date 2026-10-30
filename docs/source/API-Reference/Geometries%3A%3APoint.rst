:doc:`ArborX </index>` / :doc:`Geometries <Geometries>`


``ArborX::Point``
=================
Defined in header `<ArborX_Point.hpp> <https://github.com/arborx/ArborX/blob/master/src/geometry/ArborX_Point.hpp>`_

.. code-block:: C++

    template <int DIM, class Coordinate = float>
    struct Point;

The class template ``ArborX::Point`` defines a multi-dimensional point.


Template parameter
------------------
* ``DIM``: dimension of the point
* ``Coordinate``: type of vertex coordinates (must be default constructible)


Member types
------------
None


Member functions
----------------
+-----------------------------------------------------------------------+------------------------------------------------------------+
| :doc:`(constructor) <Geometries%3A%3APoint%3A%3APoint>`               | constructs the point                                       |
+-----------------------------------------------------------------------+------------------------------------------------------------+
| :doc:`operator[] <Geometries%3A%3APoint%3A%3Aaccess_operator>`        | returns a reference to the element at a specified location |
+-----------------------------------------------------------------------+------------------------------------------------------------+
