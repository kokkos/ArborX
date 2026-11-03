Callbacks
=========
A callback is a user-provided function object that a spatial index ``query()``
invokes, from the device, every time a value stored in the index satisfies a
predicate.
It lets you process results on the fly instead of (or in addition to) storing
them, for example to apply a filter, to compute a reduction without
materializing the results, or to transform the stored values.

ArborX does not provide a type for callbacks.
Any copyable function object (including a lambda) with a matching call
operator is a valid callback.
This page documents the requirements on such an object.

.. note::

   For ArborX 1.x, callbacks received the index of the primitive as an ``int``
   instead of the stored value. See the :doc:`migration guide <MigrationGuide>`.


Callback forms
--------------
There are two forms, selected by the ``query()`` overload the callback is
passed to.

.. list-table::
   :header-rows: 1
   :widths: 25 45 30

   * - Form
     - Call operator
     - Used by
   * - `Visiting callback`_
     - ``(Predicate, Value)``
     - ``query(space, predicates, callback)``
   * - `Output callback`_
     - ``(Predicate, Value, OutputFunctor)``
     - ``query(space, predicates, callback, values, offsets)``

In both forms,

* ``Predicate`` is the type of the predicates returned by
  :cpp:func:`AccessTraits\<Predicates\>::get() <AccessTraits::get>` after decay
  (e.g., what :cpp:func:`intersects` or
  :cpp:func:`nearest` returns).
* ``Value`` is the ``Value`` template argument of the spatial index.
* The call operator is invoked through a ``const`` reference, so it must be
  ``const``.
* The call operator is invoked from the device, so it must be marked with
  ``KOKKOS_FUNCTION``.

.. note::

   For an index built with ``ArborX::Experimental::attach_indices()``, the
   stored value is an ``ArborX::PairValueIndex``: the geometry is available as
   ``value.value`` and its position in the input as ``value.index``.
   The examples below use it.


Visiting callback
^^^^^^^^^^^^^^^^^
.. code-block:: C++

    KOKKOS_FUNCTION void operator()(Predicate const& predicate, Value const& value) const;
    KOKKOS_FUNCTION ArborX::CallbackTreeTraversalControl
    operator()(Predicate const& predicate, Value const& value) const;

Invoked once per (predicate, value) pair that satisfies the predicate (for
nearest predicates, once per each of the ``k`` nearest values).
Nothing is stored by ``query()``: the callback is the only consumer of the
results.

The return type must be

* ``void``, or
* :cpp:enum:`CallbackTreeTraversalControl` (spatial predicates only),
  to hint that the search for the current predicate may stop early.
  See `Early exit`_.

The return type of a callback used with a nearest predicate must be ``void``.


Output callback
^^^^^^^^^^^^^^^
.. code-block:: C++

    template <typename OutputFunctor>
    KOKKOS_FUNCTION void operator()(Predicate const& predicate, Value const& value,
                                    OutputFunctor const& out) const;

Invoked for every (predicate, value) pair that satisfies the predicate.
Results are stored by calling ``out(x)``, where ``x`` is convertible to the
value type of the ``values`` view passed to ``query()``.
The callback decides what is stored: ``out`` may be called any number of times,
including zero (to discard the result) or more than once (to emit several
entries per result).
The stored entries are grouped by predicate, and ``offsets`` records where the
entries of each predicate start, as for the ``{values, offsets}`` overload
without a callback.

.. warning::

   The callback may be invoked more than once for the same
   (predicate, value) pair.

   Because the size of the output is not known beforehand, ``query()`` may
   traverse the index twice: a first pass that only counts how many times
   ``out`` is called for each predicate, and a second pass that invokes the
   callback again to write the entries at the right location.
   For spatial predicates, this is the default.
   The second pass is skipped only if a buffer size was provided through
   ``ArborX::Experimental::TraversalPolicy`` and was large enough to hold all
   the entries.
   For nearest predicates, ``query()`` preallocates room for ``k`` entries per
   predicate and needs a second pass only if the callback calls ``out`` more
   times than that.

   The callback must therefore

   * call ``out`` the same number of times for the same
     (predicate, value) pair in every invocation, and
   * not rely on side effects (counters, printing, etc.) happening exactly once
     per result.

   The number of calls and their order across threads are otherwise
   unspecified. The entries of a given predicate may be stored in any order.

``OutputFunctor`` is an unspecified type, so the call operator must be a
template (or a generic lambda) on that argument.
The return type must be ``void``.

The overload of ``query()`` without a callback behaves as if it were passed the
callback

.. code-block:: C++

    struct DefaultCallback
    {
      template <typename Predicate, typename Value, typename OutputFunctor>
      KOKKOS_FUNCTION void operator()(Predicate const&, Value const& value,
                                      OutputFunctor const& out) const
      {
        out(value);
      }
    };


Early exit
^^^^^^^^^^
.. cpp:enum-class:: CallbackTreeTraversalControl

   Returned by a `visiting callback`_ for a spatial predicate to control the
   traversal of the index.

   .. cpp:enumerator:: normal_continuation

      Continue the search for the current predicate.
      This is the behavior of a callback that returns ``void``.

   .. cpp:enumerator:: early_exit

      Hint that the search for the current predicate may stop.
      The search for other predicates is not affected.

Early exit is useful when only some of the results are needed, for example to
find any (rather than all) value intersecting a region.

.. warning::

   ``early_exit`` is a hint, not a guarantee.
   The callback may still be invoked for the same predicate after it has
   returned ``early_exit``, possibly with other values and possibly
   concurrently from several threads.

   Whether the hint is honored depends on the spatial index.
   ``ArborX::BoundingVolumeHierarchy`` currently stops the traversal of a
   predicate as soon as its callback returns ``early_exit``.
   ``ArborX::BruteForce`` ignores it, as checking it would require
   synchronization between the threads working on the same predicate.

   A callback must therefore be correct if it keeps being invoked, and must not
   assume that it sees a single value per predicate.


Compile-time checks
-------------------
``query()`` diagnoses an invalid callback with a ``static_assert``:

* ``Callback 'operator()' does not have the correct signature``: the callback
  cannot be invoked with the arguments of the corresponding form above.
* ``Callback 'operator()' return type must be void``
  (or ``void or ArborX::CallbackTreeTraversalControl`` for a visiting callback
  with a spatial predicate).
* With NVCC, ``__host__ __device__ extended lambdas cannot be generic lambdas``:
  a ``KOKKOS_LAMBDA`` cannot have ``auto`` parameters when compiling with CUDA.
  Use a function object with a templated call operator instead.


Examples
--------
Store the index of every value found (output callback):

.. code-block:: C++

    struct StoreIndex
    {
      template <typename Predicate, typename Value, typename OutputFunctor>
      KOKKOS_FUNCTION void operator()(Predicate const&, Value const& value,
                                      OutputFunctor const& out) const
      {
        out(value.index);
      }
    };

    Kokkos::View<int*, MemorySpace> values("Example::values", 0);
    Kokkos::View<int*, MemorySpace> offsets("Example::offsets", 0);
    bvh.query(space, predicates, StoreIndex{}, values, offsets);

Keep only the values on one side of a plane (output callback with a filter):

.. code-block:: C++

    struct KeepPositiveX
    {
      template <typename Predicate, typename OutputFunctor>
      KOKKOS_FUNCTION void
      operator()(Predicate const&,
                 ArborX::PairValueIndex<ArborX::Point<3>, int> const& value,
                 OutputFunctor const& out) const
      {
        if (value.value[0] > 0)
          out(value.index);
      }
    };

Count the results of a spatial query without storing them (visiting callback):

.. code-block:: C++

    Kokkos::View<int, ExecutionSpace, Kokkos::MemoryTraits<Kokkos::Atomic>>
        counter("Example::counter");
    bvh.query(
        space, predicates,
        KOKKOS_LAMBDA(auto /*predicate*/, auto /*value*/) { ++counter(); });

Record one value found for each predicate (early exit):

.. code-block:: C++

    struct FindAny
    {
      // initialized to std::numeric_limits<int>::max()
      Kokkos::View<int*, MemorySpace> found;

      template <typename Predicate, typename Value>
      KOKKOS_FUNCTION auto operator()(Predicate const& predicate,
                                      Value const& value) const
      {
        // the predicates were generated with ArborX::attach(predicate, i)
        int const i = ArborX::getData(predicate);
        Kokkos::atomic_min(&found(i), value.index);
        return ArborX::CallbackTreeTraversalControl::early_exit;
      }
    };

Each predicate carries its own position ``i`` (see ``ArborX::attach()``) so that
it writes to its own entry of ``found``.
The update is atomic because the callback may be invoked concurrently for the
same predicate and may be invoked again after returning ``early_exit``.
Which of the values intersecting the predicate ends up in ``found(i)`` is
unspecified.

A complete program is available in ``examples/callback/example_callback.cpp``:

.. literalinclude:: ../../../examples/callback/example_callback.cpp
   :language: C++
   :lines: 11-


See also
--------
* :doc:`ArborX::BVH::query() <ArborX%3A%3ABoundingVolumeHierarchy%3A%3Aquery>`
* :doc:`ArborX::BruteForce::query() <ArborX%3A%3ABruteForce%3A%3Aquery>`
* :doc:`Predicates <Predicates>`
* :doc:`AccessTraits <AccessTraits>`
