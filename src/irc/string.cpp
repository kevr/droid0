/**
 * IRC string factory functions
 *
 * SPDX-License-Identifier: MIT
 **/
#include "string.hpp"
#include <sstream>

namespace droid0::irc::string
{

std::string registration(const std::string &nick, const std::string &user,
                         const std::string &realname,
                         const std::string &hostname)
{
    std::stringstream ss;
    ss << "NICK " << nick << '\n';
    ss << "USER " << user << ' ' << hostname << " * :" << realname << '\n';
    return ss.str();
}

std::string pong(const std::string &arg)
{
    std::stringstream ss;
    ss << "PONG :" << arg << '\n';
    return ss.str();
}

std::string join(const std::string &channel)
{
    std::stringstream ss;
    ss << "JOIN " << channel << '\n';
    return ss.str();
}

}; // namespace droid0::irc::string
