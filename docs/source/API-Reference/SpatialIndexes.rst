Spatial indexes
===============
ArborX provides data structures called spatial indexes which may be used to accelerate searching for objects in space.

Current version of ArborX implements two local and one distributed spatial indexes.

:doc:`ArborX::BoundingVolumeHierarchy <ArborX%3A%3ABoundingVolumeHierarchy>`

:doc:`ArborX::BruteForce <ArborX%3A%3ABruteForce>`

:doc:`ArborX::DistributedTree <ArborX%3A%3ADistributedTree>`

.. note::

   ArborX 2.0 broke backwards compatibility. For APIv1 used in ArborX 1.x, see
   :doc:`ArborX::BoundingVolumeHierarchy (v1) </API-Reference/v1/v1-ArborX%3A%3ABoundingVolumeHierarchy>` and
   :doc:`ArborX::BruteForce (v1) </API-Reference/v1/v1-ArborX%3A%3ABruteForce>`.
   Also, see :doc:`APIv2 Migration Guide <MigrationGuide>`


Complexity
----------
+---------------------------------------------------------------------------------+--------------+--------------+
| Data structure                                                                  | Construction | Query        |
+=================================================================================+==============+==============+
| :doc:`BoundingVolumeHierarchy <ArborX%3A%3ABoundingVolumeHierarchy>`            | *O(N log N)* | *O(M log N)* |
+---------------------------------------------------------------------------------+--------------+--------------+
| :doc:`BruteForce <ArborX%3A%3ABruteForce>`                                      | *O(N)*       | *O(M N)*     |
+---------------------------------------------------------------------------------+--------------+--------------+
| :doc:`DistributedTree <ArborX%3A%3ADistributedTree>`                            | *O(N log N)* | *O(M log N)* |
+---------------------------------------------------------------------------------+--------------+--------------+


Current support
---------------
+---------------------------------------------------------------------------------+-----------------+-----------------+
| Data structure                                                                  | Spatial queries | Nearest queries |
+=================================================================================+=================+=================+
| :doc:`BoundingVolumeHierarchy <ArborX%3A%3ABoundingVolumeHierarchy>`            | ✔️              | ✔️              |
+---------------------------------------------------------------------------------+-----------------+-----------------+
| :doc:`BruteForce <ArborX%3A%3ABruteForce>`                                      | ✔️              | ✔️              |
+---------------------------------------------------------------------------------+-----------------+-----------------+
| :doc:`DistributedTree <ArborX%3A%3ADistributedTree>`                            | ✔️              | ✔️              |
+---------------------------------------------------------------------------------+-----------------+-----------------+

.. toctree::
   :hidden:
   :maxdepth: 1
   :titlesonly:

   ArborX%3A%3ABoundingVolumeHierarchy
   ArborX%3A%3ABruteForce
   ArborX%3A%3ADistributedTree
