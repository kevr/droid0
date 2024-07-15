#include "listdir.hpp"
#include "../logger.hpp"
#include <cstring>
#include <dirent.h>
#include <stdexcept>
#include <sys/types.h>
#include <unistd.h>

namespace droid0::filesystem
{

std::vector<std::string> listdir(const std::string &path)
{
    std::vector<std::string> contents;

    DIR *dp = nullptr;
    if (dp = opendir(path.c_str()); dp == nullptr) {
        // unable to open directory!
        throw std::logic_error("unable to open " + path + ": " +
                               strerror(errno));
    }

    for (struct dirent *ep = readdir(dp); ep != nullptr; ep = readdir(dp)) {
        if (strcmp(ep->d_name, ".") == 0) {
            logging.debug("Skipping this dir...");
            continue;
        } else if (strcmp(ep->d_name, "..") == 0) {
            logging.debug("Skipping parent dir...");
            continue;
        }

        contents.emplace_back(ep->d_name);
    }

    closedir(dp);
    return contents;
}

}; // namespace droid0::filesystem
