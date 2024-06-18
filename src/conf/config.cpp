#include "config.hpp"
#include "../logger.hpp"

namespace po = boost::program_options;

namespace droid0
{
namespace conf
{

config::config()
    : m_cmdline_options("Command-line options"),
      m_config_options("Configuration options")
{
    std::string verbosity_help = "logging verbosity level (0 = error, 1 = "
                                 "warning, 2 = info, 3 = debug, 4 = trace)";
    m_cmdline_options.add_options()("help,h", "produce help message")(
        "version", "display application version")(
        "config,c", po::value<std::string>(), "custom configuration file")(
        "verbosity,v",
        po::value<int>()->default_value(loglevel::info)->multitoken(),
        verbosity_help.c_str())("log,l", po::value<std::string>()->multitoken(),
                                "log to a particular file");

    m_config_options.add_options()(
        "verbosity,v",
        po::value<int>()->default_value(loglevel::info)->multitoken(),
        verbosity_help.c_str())("log,l", po::value<std::string>()->multitoken(),
                                "log to a particular file");
}

config::config(const config &o)
    : m_cmdline_options(o.m_cmdline_options),
      m_config_options(o.m_config_options), m_vm(o.m_vm)
{
}

config &config::operator=(const config &o)
{
    m_vm = o.m_vm;
    return *this;
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

}; // namespace conf
}; // namespace droid0
