/**
 * Temporary directory utilities for testing purposes.
 *
 * SPDX-License-Identifier: MIT
 **/
#include "tmpdir.hpp"
#include "../logger.hpp"
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace droid0
{
namespace testutil
{

std::string mkdtemp()
{
    std::string path("/tmp/droid0-XXXXXX");
    char *ptr = const_cast<char *>(path.data());
    ::mkdtemp(ptr);
    return path;
}

void rmtree(const std::string &path)
{
    DIR *dp;
    struct dirent *ep;

    dp = opendir(path.c_str());
    if (!dp) {
        // unable to open directory!
        return;
    }

    while ((ep = readdir(dp)) != nullptr) {
        std::string full_path = path + "/" + ep->d_name;
        if (strcmp(ep->d_name, ".") == 0) {
            logging.debug("Skipping this dir...");
            continue;
        } else if (strcmp(ep->d_name, "..") == 0) {
            logging.debug("Skipping parent dir...");
            continue;
        }

        struct stat file;
        if (stat(full_path.c_str(), &file) == -1) {
            // stat error
            continue;
        }

        if (S_ISDIR(file.st_mode)) {
            logging.debug("Deleting directory: " + full_path);
            rmtree(full_path.c_str());
        } else {
            logging.debug("Deleting file: " + full_path);
            remove(full_path.c_str());
        }
    }

    closedir(dp);
    logging.debug("Deleting directory: " + path);
    rmdir(path.c_str());
}

}; // namespace testutil
}; // namespace droid0
