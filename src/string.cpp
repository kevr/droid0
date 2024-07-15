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
    strip_(
        [&]() {
            return original.front();
        },
        [&]() {
            original.erase(0, 1);
        });
    return original;
}

std::string rstrip(std::string original)
{
    strip_(
        [&]() {
            return original.back();
        },
        [&]() {
            original.pop_back();
        });
    return original;
}

std::string strip(std::string original)
{
    return lstrip(rstrip(std::move(original)));
}

std::vector<std::string> split(const std::string &orig, const char delim)
{
    std::vector<std::string> v;

    std::size_t old = 0, pos;
    while ((pos = orig.find(delim, old)) != std::string::npos) {
        v.emplace_back(orig.substr(old, pos - old));
        old = pos + 1;
    }
    v.emplace_back(orig.substr(old, orig.size() - old));

    return v;
} // LCOV_EXCL_LINE

}; // namespace droid0
