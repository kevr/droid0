// SPDX-License-Identifier: MIT
#include "stat.hpp"

namespace droid0::filesystem
{

std::tuple<struct stat, int> status(const std::string &path)
{
    struct stat st;
    int rc = stat(path.c_str(), &st);
    return std::make_tuple(st, rc);
}

bool exists(const std::string &path)
{
    auto [st, rc] = status(path);
    return rc != -1;
}

bool is_directory(const std::string &path)
{
    auto [st, rc] = status(path);
    return rc != -1 && S_ISDIR(st.st_mode);
}

}; // namespace droid0::filesystem
