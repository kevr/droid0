#ifndef FILESYSTEM_MKDIR_HPP
#define FILESYSTEM_MKDIR_HPP

#include <string>

namespace droid0::filesystem
{

//! Create directories up until and including basename
void mkdirs(const std::string &leaf);

}; // namespace droid0::filesystem

#endif /* FILESYSTEM_MKDIR_HPP */
