#include "config.hpp"
#include "../config.hpp"
#include "../environment.hpp"
#include "../logger.hpp"

namespace po = boost::program_options;

namespace droid0
{
namespace conf
{

config::config()
    : m_cmdline_options("Command-line options")
    , m_config_options("Configuration options")
{
    const std::string verbosity_help =
        "logging verbosity level (0 = error, 1 = "
        "warning, 2 = info, 3 = debug, 4 = trace)";
    const std::string default_config = xdg_config_home() + "/" + NAME + ".conf";
    m_cmdline_options.add_options()("help,h", "produce help message")(
        "version", "display application version")(
        "config,c", po::value<std::string>()->default_value(default_config),
        "custom configuration file")("make-config",
                                     "generate configuration file")(
        "verbosity,v",
        po::value<int>()->default_value(loglevel::info)->multitoken(),
        verbosity_help.c_str())("log,l", po::value<std::string>()->multitoken(),
                                "log to a particular file");

    const std::string default_network_confdir =
        xdg_config_home() + "/network.d";
    m_config_options.add_options()(
        "verbosity,v",
        po::value<int>()->default_value(loglevel::info)->multitoken(),
        verbosity_help.c_str())("log,l", po::value<std::string>()->multitoken(),
                                "log to a particular file")(
        "network_confdir",
        po::value<std::string>()->default_value(default_network_confdir),
        "configuration directory for networks");
}

config &config::parse(int argc, const char *argv[])
{
    po::store(po::parse_command_line(argc, argv, m_cmdline_options), m_vm);
    po::notify(m_vm);
    return *this;
}

config &config::parse(const std::string &filepath)
{
    po::store(po::parse_config_file(filepath.c_str(), m_config_options), m_vm);
    po::notify(m_vm);
    return *this;
}

bool config::is_set(const std::string &key) const
{
    return m_vm.count(key);
}

void config::cmdline_help(std::ostream &os, const char *progname) const
{
    os << "usage: " << progname << " [options]\n\n"
       << m_cmdline_options << "\n";
}

void write_default_config(const std::string &path)
{
    std::string content = R"(## Default configuration for droid0

## Logger verbosity (1 = error, 2 = warning, 3 = info, 4 = debug, 5 = trace)
## Default: 3
#verbosity = 3

## Path to destination logfile
#log = /path/to/droid0.log

## Path to directory housing droid0 network configurations
## Default: $XDG_CONFIG_HOME/droid0/network.d
#network_confdir = /path/to/custom/network.d

)";

    try {
        std::ofstream(path.c_str(), std::ios::out) << content;
        logging.info("wrote default config to " + path);
    } catch (const std::exception &exc) {
        logging.error("unable to write default config to " + path);
    }
}

}; // namespace conf
}; // namespace droid0
