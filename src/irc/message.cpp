// SPDX-License-Identifier: MIT
#include "message.hpp"
#include "../string.hpp"

namespace droid0::irc
{

message::message(std::string line)
    : m_raw(std::move(line))
{
    std::string arg;
    std::tie(line, arg) = message_split(m_raw);
    if (!arg.empty()) {
        m_arg = arg;
    }

    m_elements = split(strip(line));
    m_source = first();
}

const std::vector<std::string> &message::elements() const
{
    return m_elements;
}

const std::string &message::first() const
{
    return m_elements.at(0);
}

const std::string &message::source() const
{
    return m_source;
}

const std::optional<std::string> &message::arg() const
{
    return m_arg;
}

bool message::is_numeric() const
{
    return m_numeric != 0;
}

int message::numeric() const
{
    return m_numeric;
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
