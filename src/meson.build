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
  'droid0.cpp',
]

if get_option('executable')
  droid0 = executable('droid0', droid0_sources,
    install: true)
endif
