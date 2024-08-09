#include "mkdir.hpp"
#include "../logger.hpp"
#include "../string.hpp"
#include "stat.hpp"
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>

namespace droid0::filesystem
{

void mkdirs(const std::string &full_path)
{
    std::vector<std::string> parts = split(full_path, '/');
    parts.erase(parts.begin());
    std::string path;
    for (auto &part : parts) {
        path.append("/" + part);
        if (!is_directory(path)) {
            if (mkdir(path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP) == -1) {
                logging.error(strerror(errno));
                return;
            } else {
                logging.debug("mkdir('" + path + "')");
            }
        }
    }
}

}; // namespace droid0::filesystem
