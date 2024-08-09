// SPDX-License-Identifier: MIT
#include "message.hpp"
#include "../string.hpp"

namespace droid0::irc
{

message::message(std::string line)
    : std::string(std::move(line))
{
    std::string arg;
    std::tie(line, arg) = message_split(*this);
    if (!arg.empty()) {
        m_arg = arg;
    }

    m_elements = split(strip(line));

    m_source = first();
    m_source.parse();
}

const std::vector<std::string> &message::elements() const
{
    return m_elements;
}

const std::string &message::first() const
{
    return m_elements.at(0);
}

const irc::source &message::src() const
{
    return m_source;
}

const std::optional<std::string> &message::arg() const
{
    return m_arg;
}

void message::parse_command(const std::string &prefix)
{
    auto privmsg = strip(arg().value());
    if (privmsg.find(prefix) != 0)
        return;

    std::size_t o = prefix.size();
    if (auto i = privmsg.find(' ', o); i != std::string::npos) {
        // "![command] [command_arg]"
        m_command = privmsg.substr(o, i - o);
        m_command_arg = privmsg.substr(i + 1);
    } else {
        // "![command]"
        m_command = strip(privmsg).substr(o, privmsg.size() - o);
    }
}

const std::string &message::command() const
{
    return m_command;
}

const std::string &message::command_arg() const
{
    return m_command_arg;
}

std::tuple<std::string, std::string>
message::message_split(const std::string &line)
{
    auto m = line.find(':', 2);
    std::string a, b;
    if (m != std::string::npos) {
        a = line.substr(0, m);
        auto offset = m + 1;
        b = line.substr(offset, line.size() - offset);
    } else {
        a = line;
    }

    return std::make_tuple(a, b);
}

}; // namespace droid0::irc
