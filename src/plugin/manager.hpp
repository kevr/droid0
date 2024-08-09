// SPDX-License-Identifier: MIT
#ifndef PLUGIN_MANAGER_HPP
#define PLUGIN_MANAGER_HPP

#include "../conf/config.hpp"
#include "../irc/config.hpp"
#include "collection.hpp"
#include "subscription.hpp"

namespace droid0::plugin
{

class manager
{
  private:
    std::string m_network_addr = "unset";
    std::string m_network_port = "unset";

    collection m_coll;
    subscriptions<std::string> m_subs;
    std::vector<void *> m_handles;

  public:
    manager() = default;
    manager(const conf::config &cfg, const irc::config &network);
    manager(manager &&other);
    manager &operator=(manager &&other);

  public:
    void load();
    void call(const std::string &key, irc::client &client,
              const irc::message &message, void *user_data);
    void unload();
};

}; // namespace droid0::plugin

#endif /* PLUGIN_MANAGER_HPP */
