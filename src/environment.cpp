#include "environment.hpp"
#include "config.hpp"
#include <cstdlib>

namespace droid0
{

std::string xdg_config_home()
{
    std::string home(getenv("HOME"));
    return home + "/.config/" + NAME;
}

std::string xdg_data_home()
{
    std::string home(getenv("HOME"));
    return home + "/.local/share/" + NAME;
}

}; // namespace droid0
