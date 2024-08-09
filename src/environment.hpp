#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <string>

namespace droid0
{

//! $XDG_CONFIG_HOME
std::string xdg_config_home();

//! $XDG_DATA_HOME
std::string xdg_data_home();

}; // namespace droid0

#endif /* ENVIRONMENT_HPP */
