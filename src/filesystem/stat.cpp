#include "stat.hpp"
#include <cstring>
#include <stdexcept>
#include <sys/stat.h>

namespace droid0::filesystem
{

bool is_directory(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == -1) {
        throw std::runtime_error(strerror(errno));
    }
    return S_ISDIR(st.st_mode);
}

}; // namespace droid0::filesystem
