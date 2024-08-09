/**
 * IRC string factory functions
 *
 * SPDX-License-Identifier: MIT
 **/
#include "string.hpp"
#include <fmt/format.h>

namespace droid0::irc::string
{

std::string registration(const std::string &nick, const std::string &user,
                         const std::string &realname,
                         const std::string &hostname)
{
    return fmt::format("NICK {}\nUSER {} {} * :{}\n", nick, user, hostname,
                       realname);
}

std::string pong(const std::string &arg)
{
    return fmt::format("PONG :{}\n", arg);
}

std::string join(const std::string &channel)
{
    return fmt::format("JOIN {}\n", channel);
}

std::string privmsg(const std::string &dest, const std::string &text)
{
    return fmt::format("PRIVMSG {} :{}\n", dest, text);
}

}; // namespace droid0::irc::string
