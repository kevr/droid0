#ifndef FILESYSTEM_STAT_HPP
#define FILESYSTEM_STAT_HPP

#include <string>
#include <sys/stat.h>
#include <tuple>

namespace droid0::filesystem
{

//! A stat(2) wrapper
std::tuple<struct stat, int> status(const std::string &path);

//! Whether `path` exists
bool exists(const std::string &path);

//! Whether `path` is a directory
bool is_directory(const std::string &path);

}; // namespace droid0::filesystem

#endif /* FILESYSTEM_STAT_HPP */
