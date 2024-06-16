/**
 * String utilities
 *
 * SPDX-License-Identifier: MIT
 **/
#include "string.hpp"
#include <set>

using namespace droid0;

static const std::set<char> whitespace({' ', '\n', '\t'});

namespace droid0
{

std::string rstrip(std::string original)
{
    while (whitespace.find(original.back()) != whitespace.end()) {
        original.pop_back();
    }
    return original;
}

}; // namespace droid0
