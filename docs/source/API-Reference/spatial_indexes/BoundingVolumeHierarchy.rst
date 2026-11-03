``BoundingVolumeHierarchy``
===========================
Defined in header `<ArborX_LinearBVH.hpp> <https://github.com/arborx/ArborX/blob/master/src/spatial/ArborX_LinearBVH.hpp>`_

.. note::

   For ArborX 1.x, see
   :doc:`ArborX::BoundingVolumeHierarchy (v1) </API-Reference/v1/v1-ArborX%3A%3ABoundingVolumeHierarchy>`.

.. cpp:class:: template <typename MemorySpace, typename Value, \
                         typename IndexableGetter = Experimental::DefaultIndexableGetter, \
                         typename BoundingVolume = Box<DIM, Coordinate>> \
               BoundingVolumeHierarchy

   A tree data structure that can be used to accelerate the search for
   geometrical objects in space.

   :tparam MemorySpace: A valid Kokkos memory space, in which the tree resides.
   :tparam Value: The type of the objects stored in the tree.
   :tparam IndexableGetter: A function object extracting an indexable geometry
      from a ``Value``.
      The default, ``Experimental::DefaultIndexableGetter``, uses the value
      itself as the indexable, or the geometry it wraps if the value is an
      ``ArborX::PairValueIndex``.
   :tparam BoundingVolume: The bounding volume used for the internal nodes.
      By default, an ``ArborX::Box`` with the dimension and the coordinate
      type of the indexables.

   The template arguments may be deduced from the arguments of the constructor
   (see the example below).
   :cpp:type:`BVH` is a shorter name for this class.

   .. cpp:type:: memory_space = MemorySpace
   .. cpp:type:: value_type = Value
   .. cpp:type:: size_type = MemorySpace::size_type
   .. cpp:type:: bounding_volume_type = BoundingVolume

   .. cpp:function:: BoundingVolumeHierarchy() = default

      Construct an empty tree.

   .. cpp:function:: template <typename ExecutionSpace, typename Values, \
                               typename SpaceFillingCurve = Experimental::Morton64> \
                     BoundingVolumeHierarchy(ExecutionSpace const &space, \
                                             Values const &values, \
                                             IndexableGetter const &indexable_getter = IndexableGetter(), \
                                             SpaceFillingCurve const &curve = SpaceFillingCurve())

      Construct a tree from a given data source.

      :param space: The execution space in which to build the tree.
      :param values: The values to store in the tree.
      :param indexable_getter: A function object to transform a value into an
         indexable.
      :param curve: The space-filling curve used to order the values.
         The default is suitable for most uses.

      The following requirements must be satisfied.

      * ``MemorySpace`` must be accessible from ``ExecutionSpace``
        (i.e., ``Kokkos::SpaceAccessibility<ExecutionSpace, MemorySpace>::accessible``
        must be ``true``).
      * A specialization of :cpp:struct:`AccessTraits` must match ``Values``
        as the template argument.
      * The return type of :cpp:func:`AccessTraits::get` must decay to
        ``Value``.
      * ``IndexableGetter`` must provide an ``operator()`` taking in a type
        that decays to ``Value`` and returning an indexable (geometric
        object).

      The complexity is *O(N log N)*, where *N* is the number of values passed
      to the constructor (``AccessTraits<Values>::size(values)``).
      Memory allocation with Kokkos may throw.

   .. cpp:function:: KOKKOS_FUNCTION size_type size() const noexcept

      :return: The number of leaves in the tree.

   .. cpp:function:: KOKKOS_FUNCTION bool empty() const noexcept

      :return: ``true`` if the tree has no leaves, ``false`` otherwise.

   .. cpp:function:: KOKKOS_FUNCTION bounding_volume_type bounds() const noexcept

      :return: A bounding volume containing all leaves stored in the tree, or
         an invalid bounding volume if the tree is empty.

   .. cpp:function:: template <typename ExecutionSpace, typename Predicates, \
                               typename Callback> \
                     void query(ExecutionSpace const &space, \
                                Predicates const &predicates, \
                                Callback const &callback) const

      Find the values that satisfy the predicates and invoke the callback on
      each of them.

      For spatial predicates, the call finds all the values whose indexable
      meets the predicate.
      Conceptually, it is equivalent to

      .. code-block:: C++

         for (auto predicate : predicates)
           for (auto value : values)
             if (predicate(indexable_getter(value)))
               callback(predicate, value);

      For nearest predicates, the call finds the specified number of the
      values whose indexables are the nearest to the predicate geometry, and
      invokes the callback on those.
      Conceptually, it is equivalent to

      .. code-block:: C++

         for (auto predicate : predicates)
           for (auto value : k_nearest_values_of_predicate)
             callback(predicate, value);

      :param space: The execution space in which to run the query.
      :param predicates: The predicates to check against the values.
      :param callback: A function object invoked when a value satisfies a
         predicate.

   .. cpp:function:: template <typename ExecutionSpace, typename Predicates, \
                               typename Values, typename Offsets> \
                     void query(ExecutionSpace const &space, \
                                Predicates const &predicates, \
                                Values &values, Offsets &offsets) const

      Find the values that satisfy the predicates and store them in
      ``{values, offsets}``.

      ``values`` stores the values stored in the tree that satisfy the
      predicates, grouped by predicate.
      ``offsets`` stores the position in ``values`` where the entries of each
      predicate start, i.e., ``predicates(i)`` is satisfied by the values
      ``values(j)`` for ``offsets(i) <= j < offsets(i+1)``.
      Following the usual convention, ``offsets(n) == values.size()``, where
      ``n`` is the number of predicates and ``values.size()`` is the total
      number of results.
      The views are resized as needed.

      :param space: The execution space in which to run the query.
      :param predicates: The predicates to check against the values.
      :param values: The values that satisfy the predicates.
      :param offsets: The position in ``values`` of the first value of each
         predicate.

   .. cpp:function:: template <typename ExecutionSpace, typename Predicates, \
                               typename Callback, typename Values, typename Offsets> \
                     void query(ExecutionSpace const &space, \
                                Predicates const &predicates, \
                                Callback const &callback, \
                                Values &values, Offsets &offsets) const

      Find the values that satisfy the predicates, invoke the callback on each
      of them, and store in ``{values, offsets}`` what the callback outputs.
      The layout of ``values`` and ``offsets`` is the one described above.

      The callback is an *output callback*, see :doc:`/API-Reference/Callbacks`
      for how results are stored and for the number of times it may be
      invoked.

      :param space: The execution space in which to run the query.
      :param predicates: The predicates to check against the values.
      :param callback: A function object invoked when a value satisfies a
         predicate, which stores its output.
      :param values: The values output by the callback.
      :param offsets: The position in ``values`` of the first entry of each
         predicate.

   The requirements and costs below apply to all the overloads of ``query()``.

   * ``MemorySpace`` must be accessible from ``ExecutionSpace``.
   * A specialization of :cpp:struct:`AccessTraits` must match ``Predicates``
     as the template argument.
     The member type ``AccessTraits<Predicates>::memory_space`` must be
     accessible from ``ExecutionSpace``, and the return type of
     :cpp:func:`AccessTraits::get` must decay to a valid ArborX predicate,
     such as one generated by :cpp:func:`intersects` or :cpp:func:`nearest`.
   * ``Callback`` must be a valid ArborX :doc:`callback </API-Reference/Callbacks>`.
   * ``Values`` and ``Offsets`` must be (managed) Kokkos views accessible from
     ``ExecutionSpace``.

   The complexity is *O(M log N)*, where *M* is the number of predicates
   (``AccessTraits<Predicates>::size(predicates)``) and *N* is the number of
   values stored in the tree (``size()``).
   Memory allocation with Kokkos may throw.

.. cpp:type:: template <typename MemorySpace, typename Value, \
                        typename IndexableGetter = Experimental::DefaultIndexableGetter, \
                        typename BoundingVolume = Box<DIM, Coordinate>> \
              BVH = BoundingVolumeHierarchy<MemorySpace, Value, IndexableGetter, BoundingVolume>

   A convenience alias for :cpp:class:`BoundingVolumeHierarchy`, for brevity.
   The two names are interchangeable.


Example
-------
The example below builds a tree over four boxes and searches for the ones that
intersect three points.

.. literalinclude:: ../../../../examples/simple_intersection/example_intersection.cpp
   :language: C++
   :lines: 11-


See also
--------
* :doc:`Predicates </API-Reference/Predicates>`
* :doc:`Callbacks </API-Reference/Callbacks>`
* :doc:`AccessTraits </API-Reference/AccessTraits>`
* :doc:`BruteForce </API-Reference/ArborX%3A%3ABruteForce>`
