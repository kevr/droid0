#ifndef FILESYSTEM_LISTDIR_HPP
#define FILESYSTEM_LISTDIR_HPP

#include <string>
#include <vector>

namespace droid0::filesystem
{

std::vector<std::string> listdir(const std::string &path);

};

#endif /* FILESYSTEM_LISTDIR_HPP */
