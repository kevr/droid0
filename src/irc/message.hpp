// SPDX-License-Identifier: MIT
#ifndef IRC_MESSAGE_HPP
#define IRC_MESSAGE_HPP

#include "source.hpp"
#include <optional>
#include <string>
#include <tuple>
#include <vector>

namespace droid0::irc
{

class message : public std::string
{
  private:
    std::vector<std::string> m_elements;
    source m_source;
    std::optional<std::string> m_arg;
    std::string m_command;
    std::string m_command_arg;

  public:
    message(std::string line);

  public:
    //! Return internal vector
    const std::vector<std::string> &elements() const;

    //! First element of elements()
    const std::string &first() const;

    //! Source element (:source or COMMAND)
    const irc::source &src() const;

    //! Return the data's argument if it exists
    const std::optional<std::string> &arg() const;

    //! Parse message for command() and command_arg() pieces
    void parse_command(const std::string &prefix);

    //! Return command; empty string if no command
    const std::string &command() const;

    //! Return command_arg; empty string if no command_arg
    const std::string &command_arg() const;

  private:
    std::tuple<std::string, std::string> message_split(const std::string &line);
};

}; // namespace droid0::irc

#endif /* IRC_MESSAGE_HPP */
