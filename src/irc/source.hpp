// SPDX-License-Identifier: MIT
#ifndef IRC_SOURCE_HPP
#define IRC_SOURCE_HPP

#include <string>

namespace droid0::irc
{

class source : public std::string
{
  public:
    using std::string::string;
    using std::string::operator=;

    std::string m_nick;
    std::string m_user;
    std::string m_host;

  public:
    const std::string &nick() const;
    const std::string &user() const;
    const std::string &host() const;
    void parse();
};

}; // namespace droid0::irc

#endif /* IRC_SOURCE_HPP */
