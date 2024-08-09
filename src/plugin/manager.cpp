// SPDX-License-Identifier: MIT
#include "manager.hpp"
#include "../logger.hpp"
#include "../string.hpp"
#include <cstring>
#include <dlfcn.h>
#include <fmt/format.h>

namespace droid0::plugin
{

manager::manager(const conf::config &cfg, const irc::config &network)
    : m_network_addr(network.server_address())
    , m_network_port(network.server_port())
{
    const auto &plugin_dir = cfg.value<std::string>("plugin_directory");
    const auto &str_plugins = network.value<std::string>("plugins");

    auto plugins = split(str_plugins, ',');
    for (auto &plugin : plugins) {
        plugin = strip(plugin);
    }

    m_coll = collection(plugin_dir, std::move(plugins));
}

manager::manager(manager &&other)
{
    operator=(std::move(other));
}

manager &manager::operator=(manager &&other)
{
    m_network_addr = std::move(other.m_network_addr);
    m_network_port = std::move(other.m_network_port);
    m_coll = std::move(other.m_coll);
    m_subs = std::move(other.m_subs);
    m_handles = std::move(other.m_handles);
    return *this;
}

void manager::load()
{
    unload();

    std::vector<void *> handles;
    for (auto &[plugin, path] : m_coll) {
        void *handle = dlopen(path.c_str(), RTLD_LOCAL | RTLD_LAZY);
        if (!handle) {
            logging.error(fmt::format("unable to open '{}' for reading", path));
            continue;
        }

        handles.emplace_back(handle);

        // Take a pointer to `void droid0_bootstrap(...)`
        void (*droid0_bootstrap)(droid0::plugin::subscriptions<std::string> &);
        *(void **)(&droid0_bootstrap) = dlsym(handle, "droid0_bootstrap");

        // Call the bootstrap function
        std::string server(
            fmt::format("{}:{}", m_network_addr, m_network_port));
        logging.info(fmt::format("({}) loading '{}' plugin at '{}'", server,
                                 plugin, path));
        (*droid0_bootstrap)(m_subs);
        logging.info(fmt::format("({}) loaded '{}' plugin", server, plugin));
    }

    m_handles = handles;
}

void manager::call(const std::string &key, irc::client &client,
                   const irc::message &message, void *user_data)
{
    m_subs.call(key, client, message, user_data);
}

void manager::unload()
{
    for (void *handle : m_handles) {
        dlclose(handle);
    }
    m_handles.clear();
}

}; // namespace droid0::plugin
