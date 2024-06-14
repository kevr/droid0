#
# Primary meson configuration file for the project.
#
# SPDX-License-Identifier: MIT 
#
project('droid0', 'cpp',
  version: '0.0.0',
  default_options: [
    'warning_level=3',
    'cpp_std=c++14'
  ])

if get_option('tests')
  gtest_proj = subproject('gtest')
  gtest_dep = gtest_proj.get_variable('gtest_dep')
  gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')
  gmock_dep = gtest_proj.get_variable('gmock_dep')
endif

boost_dep = dependency('boost',
  modules: ['system'])

# Traverse into project srcdir
subdir('src')
