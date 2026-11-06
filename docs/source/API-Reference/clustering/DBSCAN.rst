``dbscan``
==========
Defined in header `<ArborX_DBSCAN.hpp> <https://github.com/arborx/ArborX/blob/master/src/cluster/ArborX_DBSCAN.hpp>`_

.. cpp:function:: template <typename ExecutionSpace, typename Primitives, \
                            typename Labels> \
                  void dbscan(ExecutionSpace const &space, \
                              Primitives const &primitives, double eps, \
                              int core_min_size, Labels &labels, \
                              DBSCAN::Parameters const &parameters = DBSCAN::Parameters())

   Perform DBSCAN (Density-Based Spatial Clustering of Applications with Noise
   [1]_) clustering of a set of points.

   :param space: The execution space in which to run the algorithm.
   :param primitives: The points to cluster, accessed through
      :cpp:struct:`AccessTraits`.
   :param eps: The ``eps`` parameter of the DBSCAN algorithm, i.e., the maximum
      distance between two points for them to be in the neighborhood of each
      other.
      It must be positive.
   :param core_min_size: The ``minPts`` parameter of the DBSCAN algorithm, i.e.,
      the number of points in the neighborhood of a point (itself included) for
      it to be a core point.
      It must be at least 2.
   :param labels: A one-dimensional Kokkos view of an integral type, in which
      the labels are stored.
      It is resized to the number of points.
   :param parameters: The options of the algorithm.

   The following requirements must be satisfied.

   * The memory space of ``Primitives`` must be accessible from
     ``ExecutionSpace``.
   * A specialization of :cpp:struct:`AccessTraits` must match ``Primitives``
     as the template argument, and :cpp:func:`AccessTraits::get` must return a
     :doc:`point </API-Reference/Geometries%3A%3APoint>`.
   * ``Labels`` must be a Kokkos view of an integral type, accessible from
     ``ExecutionSpace``.

   After the call, ``labels(i)`` is ``-1`` if the ``i``-th point is noise.
   Otherwise, it identifies the cluster the point belongs to: points of the same
   cluster have the same label.

   The labels are not guaranteed to be identical between runs.
   If a border point is connected to core points belonging to different
   clusters, it may be assigned to either of them.

   .. note::

      A deprecated overload of ``dbscan()``, which returns the labels instead of
      taking them as an argument, is still available.
      Please use the one above.


Parameters
----------
The options of the algorithm are passed through a ``DBSCAN::Parameters``
object, whose setters can be chained.
By default, the algorithm is DBSCAN, run with the FDBSCAN-DenseBox
implementation, without printing anything.

.. cpp:namespace:: DBSCAN

.. cpp:struct:: Parameters

   .. cpp:function:: Parameters &setVerbosity(bool verbose)

      Print statistics about the dense cells to the standard output if
      ``verbose`` is ``true``.


   .. cpp:function:: Parameters &setImplementation(Implementation impl)

      Select the implementation of the algorithm.


   .. cpp:function:: Parameters &setAlgorithm(Algorithm algorithm)

      Select the variant of the algorithm.


.. cpp:enum-class:: Implementation

   The implementation of the algorithm, see [2]_.

   .. cpp:enumerator:: FDBSCAN

      Search for the neighbors of each point with a tree.

   .. cpp:enumerator:: FDBSCAN_DenseBox

      In addition, group the points that fall in the same cell of a grid
      when the cell holds at least ``core_min_size`` points, and index each such
      dense cell as a single box.
      This is the default.

.. cpp:enum-class:: Algorithm

   The variant of the algorithm.

   .. cpp:enumerator:: DBSCAN

      Border points, i.e., points that are not core points but are in the
      neighborhood of one, are assigned to a cluster.
      This is the default.

   .. cpp:enumerator:: DBSCAN_STAR

      Border points are considered noise.


Example
-------
The example below clusters ten points in 2D with different values of ``eps`` and
``core_min_size``.

.. literalinclude:: ../../../../examples/dbscan/example_dbscan.cpp
   :language: C++
   :lines: 11-


References
----------
.. [1] Ester, M., H. P. Kriegel, J. Sander, and X. Xu, "`A Density-Based
   Algorithm for Discovering Clusters in Large Spatial Databases with Noise
   <https://dl.acm.org/doi/10.5555/3001460.3001507>`_".
   In: Proceedings of the 2nd International Conference on Knowledge Discovery
   and Data Mining, Portland, OR, AAAI Press, pp. 226-231. 1996

.. [2] Prokopenko, A., Lebrun-Grandie, D. and Arndt, A., "`Fast tree-based
   algorithms for DBSCAN for low-dimensional data on GPUs
   <https://doi.org/10.1145/3605573.3605594>`_".
   In: Proceedings of the 52nd International Conference on Parallel Processing
   (ICPP '23), Association for Computing Machinery, New York, NY, USA,
   pp. 503-512. 2023
