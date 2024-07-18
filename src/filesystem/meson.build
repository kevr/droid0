# SPDX-License-Identifier: MIT

if get_option('tests')
  filesystem_stat_test = executable('stat_test', 'stat.test.cpp',
    dependencies: test_deps)
  test('filesystem stat', filesystem_stat_test)
endif
