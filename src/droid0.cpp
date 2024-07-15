/**
 * Main entrypoint for the droid0 executable.
 *
 * SPDX-License-Identifier: MIT
 **/
#include "conf/config.hpp"
#include "config.hpp"
#include "filesystem/listdir.hpp"
#include "logger.hpp"
#include "string.hpp"
#include <iostream>
#include <libgen.h>
#include <sys/stat.h>

using namespace droid0;

int ret_error(int rc, const std::string &message);
conf::config make_config(int argc, const char **argv, int &rc);
bool set_verbosity(conf::config &);

int main(int argc, const char **argv)
{
    int rc;
    conf::config config = make_config(argc, argv, rc);
    if (rc != -1) {
        return rc;
    }

    auto network_confdir = config.value<std::string>("network_confdir");
    logging.info("network configuration directory: " + network_confdir);

    std::vector<std::string> files;
    try {
        files = filesystem::listdir(network_confdir);
    } catch (const std::logic_error &exc) {
        logging.error(exc.what());
        return 1;
    }

    return 0;
}

int ret_error(int rc, const std::string &message)
{
    std::cerr << "error: " << message << std::endl;
    return rc;
}

conf::config make_config(int argc, const char **argv, int &rc)
{
    conf::config config;
    rc = -1;
    try {
        config.parse(argc, argv);
    } catch (const boost::program_options::unknown_option &e) {
        std::string message(e.what());
        message.append(", see --help");
        rc = ret_error(1, message);
        return config;
    }

    if (config.is_set("help")) {
        config.cmdline_help(std::cout, argv[0]);
        rc = 0;
        return config;
    } else if (config.is_set("version")) {
        std::cout << VERSION << std::endl;
        rc = 0;
        return config;
    }

    if (config.is_set("make-config")) {
        const std::string dest = config.value<std::string>("config");

        // dirname
        std::string dir = dest;
        dirname(const_cast<char *>(dir.data()));

        // mkdir -p
        std::string path;
        auto parts = split(dir, '/');
        for (auto &part : parts) {
            path.push_back('/');
            path.append(part);
            mkdir(path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP);
        }

        // touch
        struct stat s;
        if (stat(dest.c_str(), &s) == -1) {
            conf::write_default_config(dest);
        }

        rc = 0;
        return config;
    }

    auto config_path = config.value<std::string>("config");
    try {
        config.parse(config_path);
    } catch (const boost::program_options::reading_file &exc) {
        std::string message(exc.what());
        message.append(", run --make-config");
        logging.error(message);
        rc = 1;
        return config;
    }

    if (config.is_set("log")) {
        logger::set_stream(config.value<std::string>("log"));
    }

    if (!set_verbosity(config)) {
        rc = ret_error(2, "invalid verbosity level");
    }

    return config;
} // LCOV_EXCL_LINE

bool set_verbosity(conf::config &config)
{
    auto verbosity = config.value<int>("verbosity");
    if (!(verbosity >= loglevel::error && verbosity <= loglevel::trace)) {
        return false;
    }

    logger::set_loglevel(static_cast<loglevel>(verbosity));
    logging.debug("set verbosity level to " + std::to_string(verbosity));
    return true;
}
