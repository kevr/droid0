#
# SPDX-License-Identifier: MIT
#

if get_option('tests')
  net_resolve_test = executable('net_resolve_test', 'resolve.test.cpp',
    dependencies: test_deps)
  test('dns resolver', net_resolve_test)
endif
