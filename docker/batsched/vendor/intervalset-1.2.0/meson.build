project('intervalset', 'cpp',
    version: '1.2.0',
    default_options: ['cpp_std=c++14']
)

# Dependencies
boost_dep = dependency('boost')
gtest_dep = dependency('gtest_main', version: '>=1.8.0', required: false)

# Building the library itself
incdir = include_directories('src')
src = ['src/intervalset.cpp', 'src/intervalset.hpp']

intervalset_shared = shared_library('intervalset',
    src,
    include_directories: [incdir],
    dependencies: [boost_dep],
    install: true, install_dir: 'lib'
)

intervalset_static = static_library('intervalset',
    src,
    include_directories: [incdir],
    dependencies: [boost_dep],
    install: true, install_dir: 'lib'
)

install_headers('src/intervalset.hpp')

# Generate a pkg-config file for the library.
pkgc = import('pkgconfig')
pkgc.generate(name: 'intervalset',
    libraries: intervalset_shared,
    version: meson.project_version(),
    # No Requires because of https://svn.boost.org/trac10/ticket/1094
    description: 'A C++ library to manage sets of closed intervals of integer.'
)

# Convenient dependency to use the shared lib from meson.
intervalset_dep = declare_dependency(
    include_directories: [incdir],
    link_with: intervalset_static,
    sources: src,
    dependencies: [boost_dep],
    version: meson.project_version()
)

# Unit tests
if gtest_dep.found()
    test_incdir = include_directories('test')
    test_src = [
        'test/tests.cpp',
        'test/access_example.cpp',
        'test/constructors_example.cpp',
        'test/membership_example.cpp',
        'test/set_operations_example.cpp',
        'test/traversal_example.cpp',
        'test/usage_example.cpp'
    ]

    unittest_exe = executable('unittest',
        test_src,
        include_directories: [test_incdir],
        dependencies: [gtest_dep, intervalset_dep]
    )
    test('unittest', unittest_exe)
endif
