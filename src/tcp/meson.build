#
# SPDX-License-Identifier: MIT
#

if get_option('tests')
  tcp_resolve_test = executable('tcp_resolve_test', 'resolve.test.cpp',
    dependencies: test_deps)
  test('dns resolver', tcp_resolve_test)

  tcp_client_test = executable('tcp_client_test', 'client.test.cpp',
    dependencies: test_deps)
  test('tcp client', tcp_client_test)
endif
