Updating GPG keys cache
=======================
Secure building of a Docker container requires a chain of trust to make sure
that any external software comes from a trusted source. Docker container is
built upon packages coming from multiple sources. Some of them, such as
packages installed from OS repository (such as Ubuntu), come with integrated
trusted keys. Others, such as boost,  are downloaded from third-party
repositories and have to be verified.

Verification of a signed package requires getting a key that the package was
signed with and checking its signature. The first step in the process is
issuing

.. code-block:: shell

    git --recv-keys [<keyserver>] <key>

Unfortunately, our CI servers experience intermittent failures accessing
external keyservers (assumingly, because of firewall related issues). As a
solution, the keys required for building containers are first manually exported
to a file hosted on internal server, and then downloaded and imported during
container build.

The commands below show the steps to create the file to export to the server:

.. code-block:: shell

    keys=(
        2D2CEF1034921684    # CMake
        379CE192D401AB61    # Boost
        86419D8A            # LLVM 1
        345AD05D            # LLVM 2
    )
    for key in ${keys[@]}; do
        gpg --recv-keys $key
    done
    gpg --armor --export ${keys[@]} > keys.gpg

If desired, the downloaded keys can then be removed:

.. code-block:: shell

    gpg --delete-keys ${keys[@]}
