``MinimumSpanningTree``
=======================
Defined in header `<ArborX_MinimumSpanningTree.hpp> <https://github.com/arborx/ArborX/blob/master/src/cluster/ArborX_MinimumSpanningTree.hpp>`_

.. warning::

   ``MinimumSpanningTree`` is experimental and its interface may change.

.. cpp:class:: template <typename MemorySpace> Experimental::MinimumSpanningTree

   The minimum spanning tree of a set of points, i.e., the set of edges that
   connect all the points with the smallest possible total weight.
   By default, the weight of an edge is the Euclidean distance between its two
   points (*Euclidean minimum spanning tree*).
   It can also be the *mutual reachability distance*, as used by
   HDBSCAN.
   The tree is computed with Boruvka's algorithm, using a
   :cpp:class:`BoundingVolumeHierarchy` to find the nearest neighbors.

   :tparam MemorySpace: The Kokkos memory space in which the edges reside.

   The class has a second, defaulted, template parameter that is an
   implementation detail used by ``hdbscan()``.

   .. cpp:type:: memory_space = MemorySpace

   .. cpp:member:: Kokkos::View<Experimental::WeightedEdge *, MemorySpace> edges

      The ``n - 1`` edges of the tree, where ``n`` is the number of points.
      The order of the edges, and which of the two points of an edge is the
      ``source`` and which is the ``target``, are unspecified.

   .. cpp:function:: template <typename ExecutionSpace, typename Primitives> \
                     MinimumSpanningTree(ExecutionSpace const &space, \
                                         Primitives const &primitives, \
                                         int k = 1)

      Compute the minimum spanning tree of a set of points.

      :param space: The execution space in which to run the algorithm.
      :param primitives: The points, accessed through
         :cpp:struct:`AccessTraits`.
      :param k: Selects the weight of the edges, see below.

      The following requirements must be satisfied.

      * ``MemorySpace`` must be the memory space of ``Primitives`` and must be
        accessible from ``ExecutionSpace``.
      * A specialization of :cpp:struct:`AccessTraits` must match ``Primitives``
        as the template argument, and
        :cpp:func:`AccessTraits\<Primitives\>::get() <AccessTraits::get>` must
        return a :doc:`point </API-Reference/Geometries%3A%3APoint>`.
      * There must be at least two points.
        Otherwise, an ``ArborX::SearchException`` is thrown.

      If ``k`` is 1 or less, the weight of the edge between the points *a* and
      *b* is the distance between them.

      If ``k`` is greater than 1, it is their *mutual reachability distance*,
      i.e., the largest of the distance between *a* and *b*, the core distance
      of *a*, and the core distance of *b*.
      The *core distance* of a point is the distance to its ``k``-th nearest
      neighbor, the point itself being the first one.
      If ``k`` is greater than the number of points, it is the distance to the
      farthest point.


.. cpp:struct:: Experimental::WeightedEdge

   An edge of a weighted graph.

   .. cpp:member:: int source
   .. cpp:member:: int target

      The indices of the two points the edge connects.

   .. cpp:member:: float weight

      The weight of the edge.

   Edges can be compared with ``operator<``, which orders them by weight and,
   for equal weights, by the smaller and then by the larger of their two
   indices.
   The comparison ignores which point is the ``source`` and which is the
   ``target``.

   Defined in header `<ArborX_WeightedEdge.hpp> <https://github.com/arborx/ArborX/blob/master/src/cluster/detail/ArborX_WeightedEdge.hpp>`_.


Example
-------
The example below computes the Euclidean minimum spanning tree of six points in
2D and prints its edges in increasing order.

.. literalinclude:: ../../../../examples/emst/example_emst.cpp
   :language: C++
   :lines: 11-


See also
--------
* :doc:`/API-Reference/AccessTraits`
