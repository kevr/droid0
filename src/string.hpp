/**
 * String utilities
 *
 * SPDX-License-Identifier: MIT
 **/
#ifndef STRING_HPP
#define STRING_HPP

#include <set>
#include <string>

namespace droid0
{

extern const std::set<char> whitespace;

template <typename ValueFunc, typename PopFunc>
void strip_(ValueFunc v, PopFunc p)
{
    while (whitespace.find(v()) != whitespace.end())
        p();
}

std::string lstrip(std::string original);
std::string rstrip(std::string original);
std::string strip(std::string original);

}; // namespace droid0

#endif /* STRING_HPP */
