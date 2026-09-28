Install
=======

From Nix
--------

| Nix_ is a package manager with amazing properties.
| It can be installed on any Linux or Unix-like (e.g., macOS) system.

.. code-block:: bash

    # Released version
    nix-env -iA intervalset \
            -f 'https://github.com/oar-team/kapack/archive/master.tar.gz'

    # Latest version
    git clone https://framagit.org/batsim/intervalset.git
    nix-env -f intervalset/ci/default.nix -i

Build It Yourself
-----------------

**intervalset** uses the Meson_ build system (and therefore Ninja_).

.. code-block:: bash

    # Get the code
    git clone https://framagit.org/batsim/intervalset.git
    cd intervalset

    # Prepare build (call meson)
    meson build # install prefix can be changed with --prefix
    cd build

    # Build the library (and the unit tests if google-test is found)
    ninja

    # Run the tests
    ninja test

    # Install
    ninja install

.. note::
   | You first need to install **intervalset**'s dependencies. Notably:

   - decent C++ compiler
   - decent Meson
   - `Boost.Icl`_
   - gtest_

   | Up-to-date dependencies are defined in the `CI Nix package`_.
   | The CI package is probably based on the `release Nix package`_.
   | Give a look at the `CI build output`_ for versions.

.. _Nix: https://nixos.org/nix/
.. _Meson: https://mesonbuild.com/
.. _Ninja: https://ninja-build.org/
.. _`Boost.Icl`: https://www.boost.org/doc/libs/release/libs/icl/doc/html/index.html
.. _`CI Nix package`: https://framagit.org/batsim/intervalset/blob/master/ci/default.nix
.. _`release Nix package`: https://github.com/oar-team/kapack/blob/master/intervalset/default.nix
.. _gtest: https://github.com/google/googletest
.. _`CI build output`: https://framagit.org/batsim/intervalset/-/jobs
