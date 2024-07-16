/**
 * String utilities
 *
 * SPDX-License-Identifier: MIT
 **/
#ifndef STRING_HPP
#define STRING_HPP

#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace droid0
{

extern const std::set<char> whitespace;

template <typename ValueFunc, typename PopFunc, typename Predicate>
void strip_(ValueFunc v, PopFunc p, Predicate pred)
{
    while (pred() && whitespace.find(v()) != whitespace.end())
        p();
}

std::string lstrip(std::string original);
std::string rstrip(std::string original);
std::string strip(std::string original);

std::vector<std::string> split(const std::string &orig, const char delim = ' ');

template <typename T>
std::string join(const std::vector<T> &orig, const std::string &delim = ",")
{
    if (!orig.size()) {
        return std::string();
    } else if (orig.size() == 1) {
        return orig.front();
    }

    std::stringstream ss;

    const auto last = orig.begin() + (orig.size() - 1);
    for (auto i = orig.begin(); i != last; ++i) {
        ss << *i << delim;
    }
    ss << orig.back();

    return ss.str();
}

}; // namespace droid0

#endif /* STRING_HPP */
