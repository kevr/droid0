// SPDX-License-Identifier: MIT
#include "source.hpp"

namespace droid0::irc
{

const std::string &source::nick() const
{
    return m_nick;
}

const std::string &source::user() const
{
    return m_user;
}

const std::string &source::host() const
{
    return m_host;
}

void source::parse()
{
    // If the message doesn't begin with :, it's not a source
    if (at(0) != ':')
        return;

    if (auto i = find('!'); i != std::string::npos) {
        m_nick = substr(1, i - 1);

        // Set new starting point
        auto o = i + 1;
        i = find('@', o);
        m_user = substr(o, i - o);

        // Set new starting point
        o = i + 1;
        m_host = substr(o, size() - o);
    } else {
        // Otherwise, it's not from a user, it's from a server
        m_host = substr(1);
    }
}

}; // namespace droid0::irc
