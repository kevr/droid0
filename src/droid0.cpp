/**
 * Main entrypoint for the droid0 executable.
 *
 * SPDX-License-Identifier: MIT
 **/
#include "conf/config.hpp"
#include "config.hpp"
#include "logger.hpp"
#include <iostream>

using namespace droid0;

int ret_error(int rc, const std::string &message);
conf::config make_config(int argc, const char **argv, int &rc);

int main(int argc, const char **argv)
{
    int rc;
    conf::config config = make_config(argc, argv, rc);

    if (rc != -1) {
        return rc;
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

    if (config.is_set("config")) {
        auto config_path = config.value<std::string>("config");
        config.parse(config_path);
    }

    if (config.is_set("log")) {
        logger::set_stream(config.value<std::string>("log"));
    }

    if (config.is_set("verbosity")) {
        auto verbosity = config.value<int>("verbosity");
        if (!(verbosity >= loglevel::error && verbosity <= loglevel::trace)) {
            rc = ret_error(2, "invalid verbosity level");
            return config;
        }

        logger::set_loglevel(static_cast<loglevel>(verbosity));
        logging.debug("set verbosity level to " + std::to_string(verbosity));
    }

    return config;
} // LCOV_EXCL_LINE
