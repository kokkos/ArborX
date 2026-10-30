:doc:`ArborX </index>` / :doc:`Geometries <Geometries>`


``ArborX::Box``
===============
Defined in header `<ArborX_Box.hpp> <https://github.com/arborx/ArborX/blob/master/src/geometry/ArborX_Box.hpp>`_

.. code-block:: C++

    template <int DIM, class Coordinate = float>
    struct Box;

The class template ``ArborX::Box`` defines a multi-dimensional axis-aligned box.


Template parameter
------------------
* ``DIM``: dimension of the box
* ``Coordinate``: type of vertex coordinates (must be default constructible)


Member types
------------
None


Member functions
----------------
+--------------------------------------------------------------+------------------------------------------+
| :doc:`(constructor) <Geometries%3A%3ABox%3A%3ABox>`          | constructs the box                       |
+--------------------------------------------------------------+------------------------------------------+
| :doc:`minCorner <Geometries%3A%3ABox%3A%3AminCorner>`        | returns the minimum (bottom left) corner |
+--------------------------------------------------------------+------------------------------------------+
| :doc:`maxCorner <Geometries%3A%3ABox%3A%3AmaxCorner>`        | returns the maximum (top right) corner   |
+--------------------------------------------------------------+------------------------------------------+
