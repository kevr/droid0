// SPDX-License-Identifier: MIT
#ifndef PLUGIN_SUBSCRIPTION_HPP
#define PLUGIN_SUBSCRIPTION_HPP

#include "../irc/client.hpp"
#include "../irc/message.hpp"
#include <functional>
#include <unordered_map>
#include <vector>

namespace droid0::plugin
{

struct subscription
{
    using function_type =
        std::function<void(irc::client &, const irc::message &, void *)>;
    function_type callback;

    subscription() = default;

    subscription(function_type cb)
        : callback(cb)
    {
    }

    subscription(const subscription &o)
        : callback(o.callback)
    {
    }
};

template <typename T>
class subscriptions
{
  private:
    std::unordered_map<T, std::vector<subscription>> m_subs;

  public:
    //! Default constructor
    subscriptions() = default;

    //! Copy constructor
    subscriptions(const subscriptions &other)
        : m_subs(other.m_subs)
    {
    }

    //! Copy assignment
    subscriptions &operator=(const subscriptions &other)
    {
        m_subs = other.m_subs;
        return *this;
    }

    //! Move constructor
    subscriptions(subscriptions &&other)
        : m_subs(std::move(other.m_subs))
    {
    }

    //! Move assignment
    subscriptions &operator=(subscriptions &&other)
    {
        m_subs = std::move(other.m_subs);
        return *this;
    }

  public:
    //! Subscribe callback to a key
    subscriptions &subscribe(const T &key, subscription::function_type callback)
    {
        m_subs[key].emplace_back(callback);
        return *this;
    }

    //! Unsubscribe callback from a key
    subscriptions &unsubscribe(const T &key,
                               const subscription::function_type &callback)
    {
        const auto &subs = m_subs[key];
        m_subs.erase(std::remove(subs.begin(), subs.end(), callback),
                     subs.end());
    }

    //! Call all subscribed callbacks of a key
    void call(const T &key, irc::client &client, const irc::message &message,
              void *data)
    {
        for (auto &sub : m_subs[key]) {
            sub.callback(client, message, data);
        }
    }
};

}; // namespace droid0::plugin

#endif /* PLUGIN_SUBSCRIPTION_HPP */
