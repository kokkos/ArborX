Running performance experiments on Summit
=========================================
Performance is considered a paramount priority for ArborX. As such, any pull
request that may affect performance has to include performance results that
indicate no regressions. So far, development of a robust system to
automatically do that has not been successful. As such, ArborX relies on
developers running the benchmarks by hand, posting the results in GitHub pull
request (PR) comments.

Our main focus is Summit supercomputer, managed by OLCF. Right now, our
performance benchmark consists of running ``bvh_driver``, which runs several
Google Benchmark tests to build and query BVH tree. Below, we document a series
of steps with a focus on avoiding making sloppy mistakes.

For our studies, we use the following configuration:

+-----------------+--------------------------------+
| Package         | Version                        |
+=================+================================+
| Host compiler   | gcc/7.4.0                      |
+-----------------+--------------------------------+
| Device compiler | cuda/10.1.243                  |
+-----------------+--------------------------------+
| MPI             | spectrum-mpi/10.3.1.2-20200121 |
+-----------------+--------------------------------+
| Boost           | 1.68.0                         |
+-----------------+--------------------------------+
| Benchmark       | 1.4                            |
+-----------------+--------------------------------+

The first step is to build Kokkos. As ArborX requires at least version 3.1.00,
this is the version we choose. Kokkos is built using the following CMake script:

.. code-block:: bash

    ARGS=(
        -D CMAKE_BUILD_TYPE=RelWithDebInfo
        -D CMAKE_INSTALL_PREFIX=$KOKKOS_INSTALL_DIR
        -D CMAKE_CXX_COMPILER="$KOKKOS_SOURCE_DIR/bin/nvcc_wrapper"
        -D Kokkos_ENABLE_SERIAL=ON
        -D Kokkos_ENABLE_OPENMP=ON
        -D Kokkos_ENABLE_CUDA=ON
            -D Kokkos_ENABLE_CUDA_LAMBDA=ON
        -D Kokkos_ENABLE_DEPRECATED_CODE=OFF
        -D Kokkos_ARCH_POWER9=ON
        -D Kokkos_ARCH_VOLTA70=ON
        )
    cmake "${ARGS[@]}" "${KOKKOS_SOURCE_DIR}"

Next, ArborX is built:

.. code-block:: bash

    ARGS=(
        -D CMAKE_BUILD_TYPE=RelWithDebInfo
        -D CMAKE_INSTALL_PREFIX=$ARBORX_INSTALL_DIR
        -D BUILD_SHARED_LIBS=ON
        -D ARBORX_ENABLE_MPI=ON
            -D MPI_EXECUTABLE=/sw/summit/xalt/1.1.3/bin/jsrun
            -D MPI_EXEC_NUMPROC_FLAG="-n"
        -D CMAKE_PREFIX_PATH="$KOKKOS_INSTALL_DIR;$BENCHMARK_INSTALL_DIR;$BOOST_INSTALL_DIR"
        -D CMAKE_CXX_COMPILER="$KOKKOS_INSTALL_DIR/bin/nvcc_wrapper"
        -D CMAKE_CXX_FLAGS="-lineinfo -DKOKKOS_ENABLE_PROFILING -Wall -Wextra -Werror"
        -D CMAKE_CXX_EXTENSIONS=OFF # required by Kokkos
        -D ARBORX_ENABLE_BENCHMARKS=ON
        )
    cmake "${ARGS[@]}" ${ARBORX_SOURCE_DIR}

The benchmark driver is located in ``benchmarks/bvh_driver`` directory.

Typically, we want to compare the performance of ``master`` branch with new
changes proposed in a PR, say in ``feature`` branch. As ArborX is header only
library, we can compile two executables, one for ``master``, one for ``feature``
and launch them in the same Summit job.

To compile ``master`` executable, proceed as follows:

.. code-block:: shell

    git checkout $(git merge-base master feature)
    cd build
    ./do-configure           # configure CMake using the above script
    cd benchmarks/bvh_driver # build only the benchmark
    make
    mv ArborX_BoundingVolumeHierarchy.exe ArborX_master_$(git rev-parse --short HEAD)
    cd -

Then, redo the process with the ``feature`` branch:

.. code-block:: shell

    git checkout feature
    cd build
    ./do-configure           # configure CMake using the above script
    cd benchmarks/bvh_driver # build only the benchmark
    make
    mv ArborX_BoundingVolumeHierarchy.exe ArborX_feature_$(git rev-parse --short HEAD)
    cd -

We note that due to certain limitations in the way current driver is
implemented, it can only run a single configuration. We add an aditional step
after checking out ``master`` or ``feature`` by running

.. code-block:: shell

    git am TEST.patch

At the moment, we have two versions of the patch: ``TEST_SETUP_NO_SORT.patch`` to
run queries without sorting, and ``TEST_SETUP_SORT.patch`` to also sort the
queries. This limitation will be addressed in the short term. These files are
provided at the bottom of this page.

At this point, one has two executables, ``ArborX_master_<hash1>`` and
``ArborX_feature_<hash2>``. The next task is to launch the job with the following script:

.. code-block:: bash

    #!/bin/bash
    ### Begin BSUB Options
    #BSUB -P PROJECT_ID
    #BSUB -J perf_test
    #BSUB -W 01:00
    #BSUB -nnodes 1
    #BSUB -alloc_flags "smt1"
    ### End BSUB Options and begin shell commands
    module load gcc/7.4.0 cuda
    labels=()
    labels+=("master_<hash1>")
    labels+=("feature_<hash2>")
    # benchmark_filter="--benchmark_filter=radius"
    jsrun_options="-n 1 -a 1 -c 42 -g 1 -r 1 -l CPU-CPU -d packed -b packed:42"
    # Exit early if executables are not found
    for label in "${labels[@]}"; do
        [ -x "./ArborX_${label}" ] || exit 1
    done
    prefix="$(date +%Y%m%d%H%M)"
    export OMP_NUM_THREADS=42
    for label in "${labels[@]}"; do
        cmd="jsrun $jsrun_options ./ArborX_${label} --benchmark_format=json --no-header --benchmark_repetitions=10 ${benchmark_filter} > ${prefix}_${label}.json"
        echo $cmd
        eval $cmd
    done

On successfull run, there are going to be two ``.json`` files in the directory.

They can then be compared using ``compare_bench.py`` supplemenatry script of
Google Benchmark.


``TEST_SETUP_NO_SORT.patch``
----------------------------
.. code-block:: diff

    diff --git a/benchmarks/bvh_driver/bvh_driver.cpp b/benchmarks/bvh_driver/bvh_driver.cpp
    index dbbc93a..8794383 100644
    --- a/benchmarks/bvh_driver/bvh_driver.cpp
    +++ b/benchmarks/bvh_driver/bvh_driver.cpp
    @@ -22,6 +22,10 @@
     #include <cstdlib>
     #include <random>

    +#ifdef ARBORX_ENABLE_MPI

    +#include <mpi.h>
    +#endif
     #include <benchmark/benchmark.h>
     #include <point_clouds.hpp>

    @@ -210,17 +214,30 @@ public:

     #define REGISTER_BENCHMARK(TreeType)                                           \
       BENCHMARK_TEMPLATE(BM_construction, TreeType)                                \

    -      ->Args({n_values, source_point_cloud_type})                              \
    +      ->Args({(int)1e4, 0})                                                    \
    +      ->Args({(int)1e5, 0})                                                    \
    +      ->Args({(int)1e6, 0})                                                    \
    +      ->Args({(int)1e4, 1})                                                    \
    +      ->Args({(int)1e5, 1})                                                    \
    +      ->Args({(int)1e6, 1})                                                    \
           ->UseManualTime()                                                        \
           ->Unit(benchmark::kMicrosecond);                                         \
       BENCHMARK_TEMPLATE(BM_knn_search, TreeType)                                  \

    -      ->Args({n_values, n_queries, n_neighbors, sort_predicates_int,           \
    -              source_point_cloud_type, target_point_cloud_type})               \
    +      ->Args({(int)1e4, (int)1e4, 10, 0, 0, 2})                                \
    +      ->Args({(int)1e5, (int)1e5, 10, 0, 0, 2})                                \
    +      ->Args({(int)1e6, (int)1e6, 10, 0, 0, 2})                                \
    +      ->Args({(int)1e4, (int)1e4, 10, 0, 1, 3})                                \
    +      ->Args({(int)1e5, (int)1e5, 10, 0, 1, 3})                                \
    +      ->Args({(int)1e6, (int)1e6, 10, 0, 1, 3})                                \
           ->UseManualTime()                                                        \
           ->Unit(benchmark::kMicrosecond);                                         \
       BENCHMARK_TEMPLATE(BM_radius_search, TreeType)                               \

    -      ->Args({n_values, n_queries, n_neighbors, sort_predicates_int,           \
    -              buffer_size, source_point_cloud_type, target_point_cloud_type})  \
    +      ->Args({(int)1e4, (int)1e4, 10, 0, 0, 0, 2})                             \
    +      ->Args({(int)1e5, (int)1e5, 10, 0, 0, 0, 2})                             \
    +      ->Args({(int)1e6, (int)1e6, 10, 0, 0, 0, 2})                             \
    +      ->Args({(int)1e4, (int)1e4, 10, 0, 0, 1, 3})                             \
    +      ->Args({(int)1e5, (int)1e5, 10, 0, 0, 1, 3})                             \
    +      ->Args({(int)1e6, (int)1e6, 10, 0, 0, 1, 3})                             \
           ->UseManualTime()                                                        \
           ->Unit(benchmark::kMicrosecond);

    @@ -270,6 +287,15 @@ public:

     int main(int argc, char *argv[])
     {
    +#ifdef ARBORX_ENABLE_MPI
    +  // Even though this benchmark does not actually use MPI, initializing the
    +  // MPI execution environment is necessary on Summit and Ascent
    +  int required = MPI_THREAD_SERIALIZED;
    +  int provided;
    +  MPI_Init_thread(&argc, &argv, required, &provided);
    +  assert(provided >= required);
    +#endif
       KokkosScopeGuard guard(argc, argv);

       namespace bpo = boost::program_options;

    @@ -361,11 +387,11 @@ int main(int argc, char *argv[])
       REGISTER_BENCHMARK(ArborX::BVH<Cuda>);
     #endif

    -#if defined(KOKKOS_ENABLE_SERIAL)

    -  REGISTER_BENCHMARK(BoostRTree);
    -#endif

       benchmark::RunSpecifiedBenchmarks();

    +#ifdef ARBORX_ENABLE_MPI

    +  MPI_Finalize();
    +#endif
       return EXIT_SUCCESS;
     }


``TEST_SETUP_SORT.patch``
-------------------------
.. code-block:: diff

    diff --git a/benchmarks/bvh_driver/bvh_driver.cpp b/benchmarks/bvh_driver/bvh_driver.cpp
    index dbbc93a..8794383 100644
    --- a/benchmarks/bvh_driver/bvh_driver.cpp
    +++ b/benchmarks/bvh_driver/bvh_driver.cpp
    @@ -22,6 +22,10 @@
     #include <cstdlib>
     #include <random>

    +#ifdef ARBORX_ENABLE_MPI

    +#include <mpi.h>
    +#endif
     #include <benchmark/benchmark.h>
     #include <point_clouds.hpp>

    @@ -210,17 +214,30 @@ public:

     #define REGISTER_BENCHMARK(TreeType)                                           \
       BENCHMARK_TEMPLATE(BM_construction, TreeType)                                \

    -      ->Args({n_values, source_point_cloud_type})                              \
    +      ->Args({(int)1e4, 0})                                                    \
    +      ->Args({(int)1e5, 0})                                                    \
    +      ->Args({(int)1e6, 0})                                                    \
    +      ->Args({(int)1e4, 1})                                                    \
    +      ->Args({(int)1e5, 1})                                                    \
    +      ->Args({(int)1e6, 1})                                                    \
           ->UseManualTime()                                                        \
           ->Unit(benchmark::kMicrosecond);                                         \
       BENCHMARK_TEMPLATE(BM_knn_search, TreeType)                                  \

    -      ->Args({n_values, n_queries, n_neighbors, sort_predicates_int,           \
    -              source_point_cloud_type, target_point_cloud_type})               \
    +      ->Args({(int)1e4, (int)1e4, 10, 1, 0, 2})                                \
    +      ->Args({(int)1e5, (int)1e5, 10, 1, 0, 2})                                \
    +      ->Args({(int)1e6, (int)1e6, 10, 1, 0, 2})                                \
    +      ->Args({(int)1e4, (int)1e4, 10, 1, 1, 3})                                \
    +      ->Args({(int)1e5, (int)1e5, 10, 1, 1, 3})                                \
    +      ->Args({(int)1e6, (int)1e6, 10, 1, 1, 3})                                \
           ->UseManualTime()                                                        \
           ->Unit(benchmark::kMicrosecond);                                         \
       BENCHMARK_TEMPLATE(BM_radius_search, TreeType)                               \

    -      ->Args({n_values, n_queries, n_neighbors, sort_predicates_int,           \
    -              buffer_size, source_point_cloud_type, target_point_cloud_type})  \
    +      ->Args({(int)1e4, (int)1e4, 10, 1, 0, 0, 2})                             \
    +      ->Args({(int)1e5, (int)1e5, 10, 1, 0, 0, 2})                             \
    +      ->Args({(int)1e6, (int)1e6, 10, 1, 0, 0, 2})                             \
    +      ->Args({(int)1e4, (int)1e4, 10, 1, 0, 1, 3})                             \
    +      ->Args({(int)1e5, (int)1e5, 10, 1, 0, 1, 3})                             \
    +      ->Args({(int)1e6, (int)1e6, 10, 1, 0, 1, 3})                             \
           ->UseManualTime()                                                        \
           ->Unit(benchmark::kMicrosecond);

    @@ -270,6 +287,15 @@ public:

     int main(int argc, char *argv[])
     {
    +#ifdef ARBORX_ENABLE_MPI
    +  // Even though this benchmark does not actually use MPI, initializing the
    +  // MPI execution environment is necessary on Summit and Ascent
    +  int required = MPI_THREAD_SERIALIZED;
    +  int provided;
    +  MPI_Init_thread(&argc, &argv, required, &provided);
    +  assert(provided >= required);
    +#endif
       KokkosScopeGuard guard(argc, argv);

       namespace bpo = boost::program_options;

    @@ -361,11 +387,11 @@ int main(int argc, char *argv[])
       REGISTER_BENCHMARK(ArborX::BVH<Cuda>);
     #endif

    -#if defined(KOKKOS_ENABLE_SERIAL)

    -  REGISTER_BENCHMARK(BoostRTree);
    -#endif

       benchmark::RunSpecifiedBenchmarks();

    +#ifdef ARBORX_ENABLE_MPI

    +  MPI_Finalize();
    +#endif
       return EXIT_SUCCESS;
     }
