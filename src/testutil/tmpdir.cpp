/**
 * Temporary directory utilities for testing purposes.
 *
 * SPDX-License-Identifier: MIT
 **/
#include "tmpdir.hpp"
#include "../filesystem/listdir.hpp"
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
    auto files = filesystem::listdir(path);
    for (const auto &file : files) {
        std::string full_path = path + "/" + file;

        struct stat fs;
        stat(full_path.c_str(), &fs);
        if (S_ISDIR(fs.st_mode)) {
            logging.debug("Deleting directory: " + full_path);
            rmtree(full_path.c_str());
        } else {
            logging.debug("Deleting file: " + full_path);
            remove(full_path.c_str());
        }
    }

    logging.debug("Deleting directory: " + path);
    rmdir(path.c_str());
}

}; // namespace testutil
}; // namespace droid0
