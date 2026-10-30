Build
=====


Dependencies
------------
+-----------------------------------------------+---------+----------+---------+
| Dependency                                    | Version | Required | Runtime |
+===============================================+=========+==========+=========+
| `Kokkos  <https://github.com/kokkos/kokkos>`_ | 4.5.00  | ✔️       | ✔️      |
+-----------------------------------------------+---------+----------+---------+
| MPI                                           | 2       | ✔️       |         |
+-----------------------------------------------+---------+----------+---------+
| CMake                                         | 3.22    | ✔️       |         |
+-----------------------------------------------+---------+----------+---------+


Build and installation
----------------------
The build instructions for the Kokkos library can be found
`here  <https://github.com/kokkos/kokkos#building-and-installing-kokkos>`_.
ArborX requires Kokkos CMake build to have ``Kokkos_ENABLE_CUDA_LAMBDA=ON`` if
``Kokkos_ENABLE_CUDA=ON``.


Example: `OLCF Frontier  <https://docs.olcf.ornl.gov/systems/frontier_user_guide.html>`_
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To build Kokkos on Frontier (AMD EPYC 3rd gen CPU and AMD 250X) with three
backends (Serial, OpenMP and HIP), configure it with

.. code-block:: bash

    OPTIONS=(
        -D CMAKE_INSTALL_PREFIX="${KOKKOS_INSTALL_DIR}"
        -D CMAKE_CXX_COMPILER=hipcc
        -D Kokkos_ENABLE_SERIAL=ON
        -D Kokkos_ENABLE_OPENMP=ON
        -D Kokkos_ENABLE_HIP=ON
            -D Kokkos_ENABLE_HIP_MULTIPLE_KERNEL_INSTANTIATIONS=ON
        -D Kokkos_ARCH_ZEN2=ON
        -D Kokkos_ARCH_VEGA90A=ON
        )
    cmake "${OPTIONS[@]}" "${KOKKOS_SOURCE_DIR:-../}"


Example: `ALCF Aurora  <https://docs.alcf.anl.gov/aurora/>`_
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To build Kokkos on Aurora (Intel Xeon Saphire Rapids with Intel Pointe Vecchio) with two backends (OpenMP and SYCL), configure it with

.. code-block:: bash

    OPTIONS=(
        -D CMAKE_INSTALL_PREFIX="${KOKKOS_INSTALL_DIR}"
        -D CMAKE_CXX_COMPILER=icpx
        -D CMAKE_CXX_FLAGS="-ffp-model=precise -fsycl-device-code-split=per_kernel"
        -D CMAKE_EXE_LINKER_FLAGS="-ffp-model=precise"
        -D Kokkos_ENABLE_OPENMP=ON
        -D Kokkos_ENABLE_SYCL=ON
        -D Kokkos_ARCH_INTEL_PVC=ON
        )
    cmake "${OPTIONS[@]}" "${KOKKOS_SOURCE_DIR:-../}"


Example: `NERSC Perlmutter  <https://docs.nersc.gov/systems/perlmutter/>`_
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To build Kokkos on Perlmutter (AMD EPYC 7763 CPU with Nvidia A100 GPU) with two
backends (OpenMP and CUDA), configure it with

.. code-block:: bash

    OPTIONS=(
        -D CMAKE_INSTALL_PREFIX="${KOKKOS_INSTALL_DIR}"
        -D CMAKE_CXX_EXTENSIONS=OFF
        -D CMAKE_CXX_COMPILER="${KOKKOS_SOURCE_DIR}/bin/nvcc_wrapper"
        -D Kokkos_ENABLE_OPENMP=ON
        -D Kokkos_ENABLE_CUDA=ON
            -D Kokkos_ENABLE_CUDA_LAMBDA=ON
        -D Kokkos_ARCH_ZEN3=ON
        -D Kokkos_ARCH_AMPERE80=ON
        )
    cmake "${OPTIONS[@]}" "${KOKKOS_SOURCE_DIR:-../}"


CMake
~~~~~
Assuming that Kokkos library is installed in ``$KOKKOS_INSTALL_DIR``, configure
ArborX as

.. code-block:: bash

    OPTIONS=(
        -D CMAKE_INSTALL_PREFIX="${ARBORX_INSTALL_DIR}"
        -D ARBORX_ENABLE_MPI=ON
        -D Kokkos_ROOT="${KOKKOS_INSTALL_DIR}"
        -D CMAKE_CXX_COMPILER="${KOKKOS_INSTALL_DIR}/bin/nvcc_wrapper" # only for CUDA-enabled Kokkos
        -D CMAKE_CXX_COMPILER=hipcc # for HIP-enabled Kokkos
        -D CMAKE_CXX_COMIPLER=icpx  # for SYCL-enabled Kokkos
        -D CMAKE_CXX_EXTENSIONS=OFF # required by Kokkos
        )
    cmake "${OPTIONS[@]}" "${ARBORX_DIR:-../}"

then run ``make install``. ArborX will then be installed in
``$ARBORX_INSTALL_DIR``.

To disable MPI, configure with

.. code-block:: bash

        -D ARBORX_ENABLE_MPI=OFF

For GPU-aware MPI, configure with

.. code-block:: bash

        -D ARBORX_ENABLE_GPU_AWARE_MPI=ON

.. note::

   On Frontier, this requires additionally loading ``craype-accel-amd-gfx90a`` and adding ``MPICH_GPU_SUPPORT_ENABLED=1`` environment variable

To enable ArborX examples, configure with

.. code-block:: bash

        -D ARBORX_ENABLE_EXAMPLES=ON

To validate the ArborX build, install Boost (versions 1.67, 1.68, or 1.75+)
(``$BOOST_INSTALL_DIR``), configure with

.. code-block:: bash

        -D Kokkos_ROOT="${KOKKOS_INSTALL_DIR}"
        -D Boost_ROOT="${BOOST_INSTALL_DIR}"
        -D ARBORX_ENABLE_TESTS=ON

and run ``ctest`` after completing the build.

To enable ArborX benchmarks, install
`Google Benchmark <https://github.com/google/benchmark>`_ version 1.5.4 or later
(``$BENCHMARK_INSTALL_DIR``), Boost version 1.67 or later (``$BOOST_INSTALL_DIR``),
configure with

.. code-block:: bash

        -D Kokkos_ROOT="${KOKKOS_INSTALL_DIR}"
        -D Boost_ROOT="${BOOST_INSTALL_DIR}"
        -D benchmark_ROOT="${BENCHMARK_INSTALL_DIR}"
        -D ARBORX_ENABLE_BENCHMARKS=ON

The individual benchmarks can then be run from ``benchmarks`` directory.

.. note::

   If benchmark is not not found, but ``ARBORX_ENABLE_BENCHMARKS=ON``, it will be downloaded and built automatically.

ArborX also supports building against Kokkos built as part of
`Trilinos  <https://github.com/trilinos/Trilinos>`_. This requires Trilinos 16.2
or later. In this case, ``$KOKKOS_INSTALL_DIR`` should point to the Trilinos
installation directory.


Spack
~~~~~
ArborX can also be installed using the `Spack  <https://github.com/spack/spack>`_
package manager. A basic installation can be done as:

.. code-block:: shell

    spack install arborx

Spack allows options and compilers to be turned on in the install command:

.. code-block:: shell

    spack install arborx@1.0 %gcc@8.1.0 ~mpi+cuda cuda_arch=70

This example illustrates the most common parameters:
- Variants are specified with ``~`` or ``+`` (e.g. ``+cuda``) and enable or disable certain options
- Version (``@version``) immediately follows ``arborx`` and can specify a specific ArborX version
- Compiler (``%compiler``) can be set if an exact compiler is desired; otherwise, a default compiler is chosen

.. note::

   Building with CUDA (``+cuda``) requires ``cuda_arch`` speicification. Building with ROCm (``+rocm``) requires ``amdgpu_target`` specification.

For a complete list of available ArborX options, run:

.. code-block:: shell

    spack info arborx


Building against ArborX
-----------------------
For projects using CMake, add to ``CMakeLists.txt``

.. code-block:: bash

    find_package(ArborX REQUIRED)

For any targets requiring ArborX, add

.. code-block:: bash

    target_link_libraries(<target> ArborX::ArborX)

Note that because of the way CMake deals with dependencies, users of ArborX
will need to make sure that Kokkos installation directory is visible to CMake.
The easiest way to address it is to add both to the configuration options:

.. code-block:: bash

        -D CMAKE_PREFIX_PATH="$ARBORX_INSTALL_DIR;$KOKKOS_INSTALL_DIR"
