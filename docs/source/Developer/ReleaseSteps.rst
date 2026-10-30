Steps for a new ArborX release
==============================
1. Make sure ``master`` passes all tests.
2. Make sure all files have correct copyright headers (including correct years).
3. Update CHANGELOG file.
4. Create release branch ``arborx-a.b``.
5. Update ``ARBORX_VERSION_STRING`` in the ``master`` branch.
6. Update ``ARBORX_VERSION_STRING`` in the release branch.
7. Update hard-coded version in ``examples/CMakeLists.txt`` in the ``master``
   branch.
8. Test release branch with Spack.
9. Create GitHub release from the release branch.
10. Remove the release branch.
11. Create Spack PR to include new release.
12. After merging Spack PR, notify xSDK to update ArborX version.
13. Celebrate!
