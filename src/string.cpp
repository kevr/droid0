/**
 * String utilities
 *
 * SPDX-License-Identifier: MIT
 **/
#include "string.hpp"

namespace droid0
{

const std::set<char> whitespace{' ', '\r', '\n', '\t'};

std::string lstrip(std::string original)
{
    strip_([&]() { return original.front(); }, [&]() { original.erase(0, 1); });
    return original;
}

std::string rstrip(std::string original)
{
    strip_([&]() { return original.back(); }, [&]() { original.pop_back(); });
    return original;
}

std::string strip(std::string original)
{
    return lstrip(rstrip(std::move(original)));
}

}; // namespace droid0
