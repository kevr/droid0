#include "config.hpp"
#include "../string.hpp"
#include <fstream>

namespace po = boost::program_options;

namespace droid0::irc
{

config::config(const std::string &config_file)
    : m_desc("Network Configuration")
    , m_vm(std::make_unique<boost::program_options::variables_map>())
{
    m_desc.add_options()("server_address", po::value<std::string>()->required(),
                         "remote server address");
    m_desc.add_options()("server_port", po::value<std::string>()->required(),
                         "remote server port");
    m_desc.add_options()("nick", po::value<std::string>()->required(),
                         "nickname");
    m_desc.add_options()("user", po::value<std::string>()->required(),
                         "username");
    m_desc.add_options()("realname", po::value<std::string>()->required(),
                         "real name");
    m_desc.add_options()("channels", po::value<std::string>(),
                         "channels (# excluded) separated by commas");
    m_desc.add_options()("prefix", po::value<std::string>()->default_value("!"),
                         "command prefix (single character)");
    m_desc.add_options()("plugins", po::value<std::string>(),
                         "plugin names separated by commas");

    std::ifstream file(config_file.c_str());
    po::store(po::parse_config_file(file, m_desc), *m_vm);
    po::notify(*m_vm);
}

config::config(const config &other)
    : m_desc(other.m_desc)
    , m_vm(other.m_vm)
{
}

config &config::operator=(const config &other)
{
    m_vm = other.m_vm;
    return *this;
}

bool config::is_set(const std::string &key)
{
    return m_vm->count(key) > 0;
}

const std::string &config::server_address() const
{
    return value<std::string>("server_address");
}

const std::string &config::server_port() const
{
    return value<std::string>("server_port");
}

const std::string &config::nick() const
{
    return value<std::string>("nick");
}

const std::string &config::user() const
{
    return value<std::string>("user");
}

const std::string &config::realname() const
{
    return value<std::string>("realname");
}

const std::string &config::channels() const
{
    return value<std::string>("channels");
}

const std::string &config::prefix() const
{
    return value<std::string>("prefix");
}

std::vector<std::string> config::plugins() const
{
    return split(value<std::string>("plugins"), ',');
}

}; // namespace droid0::irc
