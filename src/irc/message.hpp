// SPDX-License-Identifier: MIT
#ifndef IRC_MESSAGE_HPP
#define IRC_MESSAGE_HPP

#include <optional>
#include <string>
#include <tuple>
#include <vector>

namespace droid0::irc
{

class message
{
  private:
    std::string m_raw;
    std::vector<std::string> m_elements;
    std::string m_source;
    std::optional<std::string> m_arg;

    int m_numeric = 0;

  public:
    message(std::string line);

  public:
    //! Return internal vector
    const std::vector<std::string> &elements() const;

    //! First element of elements()
    const std::string &first() const;

    //! Source element (:source or COMMAND)
    const std::string &source() const;

    const std::optional<std::string> &arg() const;

    //! Return if this message is numeric
    bool is_numeric() const;

    //! Return numeric -- !is_numeric() => 0
    int numeric() const;

  private:
    std::tuple<std::string, std::string> message_split(const std::string &line);
};

}; // namespace droid0::irc

#endif /* IRC_MESSAGE_HPP */
