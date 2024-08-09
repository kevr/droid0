// SPDX-License-Identifier: MIT
#include "collection.hpp"

namespace droid0::plugin
{

collection::collection(const std::string &dir, std::vector<std::string> il)
    : m_dir(dir)
{
    for (const auto &plugin : il) {
        m_plugins.emplace_back(
            std::make_pair(plugin, m_dir + "/" + plugin + ".so"));
    }
}

collection::collection(const collection &other)
{
    operator=(other);
}

collection &collection::operator=(const collection &other)
{
    m_dir = other.m_dir;
    m_plugins = other.m_plugins;
    return *this;
}

std::vector<string_pair>::iterator collection::begin()
{
    return m_plugins.begin();
}

std::vector<string_pair>::const_iterator collection::cbegin() const
{
    return m_plugins.cbegin();
}

std::vector<string_pair>::iterator collection::end()
{
    return m_plugins.end();
}

std::vector<string_pair>::const_iterator collection::cend() const
{
    return m_plugins.cend();
}

}; // namespace droid0::plugin
