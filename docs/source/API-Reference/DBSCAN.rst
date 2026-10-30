:doc:`ArborX </index>`


``ArborX::dbscan``
==================
Defined in header `<ArborX_DBSCAN.hpp> <https://github.com/arborx/ArborX/blob/master/src/cluster/ArborX_DBSCAN.hpp>`_

.. code-block:: C++

    template <typename ExecutionSpace, typename Primitives>
    Kokkos::View<int *, typename ArborX::AccessTraits<Primitives>::memory_space>
    dbscan(ExecutionSpace const &space, Primitives const &primitives, float eps, int core_min_size);

The free function ``ArborX::dbscan`` performs DBSCAN (Density-Based Spatial Clustering of Applications with Noise) clustering from a set of points.


Parameters
----------
* ``space``: the execution space.
* ``primitives``: geometrical objects one wishes to index. The return value of ``ArborX::AccessTraits<Primitives>::get()`` must decay to ``ArborX::Point``.
* ``eps``: The ``eps`` parameter in the DBSCAN algorithm, corresponding to the maximum distance between two objects to be considered in the neighborhood of each other.
* ``core_min_size``: The ``minPts`` parameter in the DBSCAN algorithm, corresponding to the number of objects in the neighborhood for a point to be considered a core point.


Returns
-------
Cluster labels for each point in the dataset. Noise points are given the label -1.


Example
-------
.. code-block:: C++

    #include <ArborX_DBSCAN.hpp>

    #include <iostream>

    int main(int argc, char *argv[])
    {
      Kokkos::ScopeGuard guard(argc, argv);

      Kokkos::View<ArborX::Point *> cloud("point_cloud", 9);
      auto cloud_host = Kokkos::create_mirror_view(cloud);
      //     0 1 2 3 4
      //    ----------
      // 3 |     0 7
      // 2 |     5 6
      // 1 | 4 3
      // 0 | 1 2     8
      cloud_host[0] = {3.f, 2.f, 0.f};
      cloud_host[1] = {0.f, 0.f, 0.f};
      cloud_host[2] = {0.f, 1.f, 0.f};
      cloud_host[3] = {1.f, 1.f, 0.f};
      cloud_host[4] = {1.f, 0.f, 0.f};
      cloud_host[5] = {2.f, 2.f, 0.f};
      cloud_host[6] = {2.f, 3.f, 0.f};
      cloud_host[7] = {3.f, 3.f, 0.f};
      cloud_host[8] = {4.f, 0.f, 0.f};
      Kokkos::deep_copy(cloud, cloud_host);

      using execution_space = decltype(cloud)::execution_space; // where to execute code
      auto labels = ArborX::dbscan(execution_space{}, cloud, 1.2, 2);

      auto labels_host = Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace{}, labels);
      for (int i = 0; i < (int)labels_host.size(); ++i)
        std::cout << labels_host(i) << " ";
      std::cout << std::endl;

      return 0;
    }

Output

.. code-block:: text

    0 1 1 1 1 0 0 0 -1


Notes
-----
The resulting labels are not guaranteed to be identical between the runs. If a border point is connected to core points belonging to different clusters, it may be assigned to either of them.


References
----------
Ester, M., H. P. Kriegel, J. Sander, and X. Xu, “`A Density-Based Algorithm for Discovering Clusters in Large Spatial Databases with Noise <https://dl.acm.org/doi/10.5555/3001460.3001507>`_”. In: Proceedings of
the 2nd International Conference on Knowledge Discovery and Data Mining,
Portland, OR, AAAI Press, pp. 226-231. 1996
