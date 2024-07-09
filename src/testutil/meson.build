#
# Internal testing library with testing bits of source code.
#
# SPDX-License-Identifier: MIT
#

if get_option('tests')

  testlib_sources = [
    'tmpdir.cpp',
    '../tcp/server.cpp',
    '../tcp/connection.cpp',
  ]
  droid0_testlib = static_library('droid0_test', testlib_sources)
  droid0_testdep = declare_dependency(link_with: droid0_testlib)

  test_deps = droid0_deps + [droid0_testdep, gtest_dep, gtest_main_dep]
  mock_deps = test_deps + [gmock_dep]

  tmpdir_test = executable('tmpdir.test', 'tmpdir.test.cpp',
    dependencies: test_deps)
  test('tmpdir', tmpdir_test)

endif
