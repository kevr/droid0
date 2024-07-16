#
# Primary source compilation configuration for the meson project
#
# SPDX-License-Identifier: MIT
#

conf_data = configuration_data()
conf_data.set('version', get_option('version'))
configure_file(input: 'config.hpp.in',
               output: 'config.hpp',
               configuration: conf_data)
message('- Dynamic version:', get_option('version'))

# Non-test sources for the droid0 executable
droid0_sources = [
  'environment.cpp',
  'logger.cpp',
  'string.cpp',
  'conf/config.cpp',
  'filesystem/listdir.cpp',
  'filesystem/stat.cpp',
  'irc/config.cpp',
  'irc/message.cpp',
  'irc/string.cpp',
  'tcp/client.cpp',
  'tcp/resolve.cpp',
  'tcp/errors.cpp',
]
droid0_lib = static_library('droid0', droid0_sources)
droid0_dep = declare_dependency(link_with: droid0_lib)
droid0_deps = [droid0_dep, boost_dep]

if get_option('executable')
  droid0 = executable('droid0', 'droid0.cpp',
    dependencies: droid0_deps,
    install: true)
endif

if get_option('tests')
  # Traverse into testing to build our testlib
  subdir('testutil')

  droid0_test = executable('droid0.test', 'droid0.test.cpp',
    dependencies: test_deps)
  test('main entrypoint', droid0_test)

  logger_test = executable('logger.test', 'logger.test.cpp',
    dependencies: test_deps)
  test('logger', logger_test)

  string_test = executable('string.test', 'string.test.cpp',
    dependencies: test_deps)
  test('string', string_test)

  # Delve into deeper tests
  subdir('tcp')
  subdir('irc')
endif
