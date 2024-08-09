// SPDX-License-Identifier: MIT
#ifndef IRC_OPCODE_HPP
#define IRC_OPCODE_HPP

#include <string>

namespace droid0::irc
{

//! Numeric opcodes
enum opcode : unsigned int
{
    connected = 001,
    joined = 353,
};

//! Return a 3-length 0-padded string version of `op`
std::string to_string(opcode op);

}; // namespace droid0::irc

#endif /* IRC_OPCODE_HPP */
