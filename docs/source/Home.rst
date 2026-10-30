ArborX: a performance portable geometric search library
=======================================================
The ArborX library allows users to search for nearby geometric objects in a performance portable way. By geometric search, we mean the task of finding the nearest neighbors of an object, or finding all objects within a certain distance. These tasks are often called k-nearest neighbor search (kNN) and radius-based search. ArborX is designed to run on a variety of hardware, from commodity CPUs to specialized accelerators (GPUs), all the while providing high performance. ArborX relies on `Kokkos  <https://github.com/kokkos/kokkos>`_ for a programming model abstraction.

This documentation provides resources for ArborX users and developers.


:doc:`Build`
------------
Here you can find instructions on building ArborX.


:doc:`API Reference <API-Reference>`
----------------------------------------
Here you can learn the syntax of functions and classes.


:doc:`Glossary`
---------------
Here you can find the meaning of terms used in the API descriptions.

.. toctree::
   :hidden:
   :maxdepth: 1
   :titlesonly:
   :caption: Documentation

   Build
   Glossary
   API-Reference/MigrationGuide
   API-Reference
   API-Reference v1 <API-Reference/v1/index>
   Developer/index
