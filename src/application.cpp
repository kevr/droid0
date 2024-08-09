// SPDX-License-Identifier: MIT
#include "application.hpp"
#include "config.hpp"
#include "filesystem/dirname.hpp"
#include "filesystem/listdir.hpp"
#include "filesystem/mkdir.hpp"
#include "filesystem/stat.hpp"
#include "irc/client.hpp"
#include "logger.hpp"
#include "string.hpp"
#include "tcp/resolve.hpp"
#include <iostream>

static int ret_error(int rc, const std::string &message)
{
    std::cerr << "error: " << message << std::endl;
    return rc;
}

namespace droid0
{

int application::run(int argc, const char *argv[])
{
    if (int rc = parse_config(argc, argv); rc != -1) {
        return rc;
    } else {
        logging.info("loaded configuration: '" + config.path() + "'");
    }

    auto plugin_directory = config.value<std::string>("plugin_directory");
    filesystem::mkdirs(plugin_directory);

    auto network_confdir = config.value<std::string>("network_confdir");
    filesystem::mkdirs(network_confdir);
    logging.info("network configuration directory: '" + network_confdir + "'");

    std::vector<std::string> files;
    try {
        files = filesystem::listdir(network_confdir);
    } catch (const std::logic_error &exc) {
        logging.error(exc.what());
        return 1;
    }

    std::vector<irc::config> networks;
    for (auto it = files.begin(); it != files.end(); ++it) {
        std::vector<std::string> v{network_confdir, *it};
        const auto path = join(v, "/");
        if (!filesystem::is_directory(path)) {
            networks.emplace_back(path);
            logging.info("loaded network '" + path + "'");
        }
    }

    boost::asio::io_service io;
    std::vector<std::unique_ptr<irc::client>> clients;
    for (auto &network : networks) {
        try {
            clients.emplace_back(std::make_unique<irc::client>(
                io, network.nick(), network.user(), network.realname()));
        } catch (const std::exception &exc) {
            logging.error(exc.what());
            continue;
        }

        auto &client = clients.back();
        {
            auto channels = split(network.channels(), ',');
            std::transform(channels.begin(), channels.end(),
                           std::back_inserter(client->channels()),
                           [](const std::string &s) {
                               return "#" + s;
                           });
        }

        client->bootstrap(config, network);

        auto results =
            tcp::resolve(io, network.server_address(), network.server_port());
        client->start(results);
    }

    io.run();
    return 0;
}

int application::parse_config(int argc, const char *argv[])
{
    try {
        config.parse(argc, argv);
    } catch (const boost::program_options::unknown_option &e) {
        std::string message(e.what());
        message.append(", see --help");
        return ret_error(1, message);
    }

    if (config.is_set("help")) {
        config.cmdline_help(std::cout, argv[0]);
        return 0;
    } else if (config.is_set("version")) {
        std::cout << VERSION << std::endl;
        return 0;
    }

    if (config.is_set("make-config")) {
        const std::string dest = config.value<std::string>("config");

        // dirname
        std::string dir = filesystem::dirname(dest);

        // mkdir -p
        filesystem::mkdirs(dir);

        // touch
        if (!filesystem::exists(dest)) {
            conf::write_default_config(dest);
        }

        return 0;
    }

    auto config_path = config.value<std::string>("config");
    try {
        config.parse(config_path);
    } catch (const boost::program_options::reading_file &exc) {
        std::string message(exc.what());
        message.append(", run --make-config");
        logging.error(message);
        return 1;
    }

    if (config.is_set("log")) {
        logger::set_stream(config.value<std::string>("log"));
    }

    if (!set_verbosity()) {
        return ret_error(2, "invalid verbosity level");
    }

    return -1;
}

bool application::set_verbosity()
{
    auto verbosity = config.value<int>("verbosity");
    if (!(verbosity >= loglevel::error && verbosity <= loglevel::trace)) {
        return false;
    }

    logger::set_loglevel(static_cast<loglevel>(verbosity));
    logging.debug("set verbosity level to " + std::to_string(verbosity));
    return true;
}

}; // namespace droid0
