/**
 * IRC string factory functions
 *
 * SPDX-License-Identifier: MIT
 **/
#ifndef IRC_STRING_HPP
#define IRC_STRING_HPP

#include <string>

namespace droid0::irc::string
{

std::string registration(const std::string &nick, const std::string &user,
                         const std::string &realname,
                         const std::string &hostname = "*");

std::string pong(const std::string &arg);
std::string join(const std::string &channel);
std::string privmsg(const std::string &dest, const std::string &text);

}; // namespace droid0::irc::string

#endif /* IRC_STRING_HPP */
