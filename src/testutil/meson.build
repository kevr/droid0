#
# Internal testing library with testing bits of source code.
#
# SPDX-License-Identifier: MIT
#

if get_option('tests')

  testlib_sources = [
    'tmpdir.cpp',
  ]
  droid0_testlib = static_library('droid0_test', testlib_sources)
  droid0_testdep = declare_dependency(link_with: droid0_testlib)

endif
