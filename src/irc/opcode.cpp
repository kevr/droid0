// SPDX-License-Identifier: MIT
#include "opcode.hpp"
#include <fmt/format.h>

namespace droid0::irc
{

std::string to_string(opcode op)
{
    return fmt::format("{:03}", static_cast<int>(op));
}

}; // namespace droid0::irc
