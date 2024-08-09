#include "../config.hpp"
#include "../irc/client.hpp"
#include "../irc/message.hpp"
#include "../logger.hpp"
#include "../plugin/plugin.hpp"

using namespace droid0;
using namespace std::string_literals;

// Constants
static const std::string GITHUB_URL = "https://github.com/kevr/droid0"s;

// Plugin code
void print_log(const std::string &message);
void handle_version(irc::client &client, const irc::message &message, void *);
void handle_source(irc::client &client, const irc::message &message, void *);

void droid0_bootstrap(droid0::plugin::subscriptions<std::string> &subs)
{
    subs.subscribe("version", handle_version);
    subs.subscribe("source", handle_source);
}

void print_log(const std::string &message)
{
    logging.info("(plugin:core) " + message);
}

void handle_version(irc::client &client, const irc::message &message, void *)
{
    print_log("(plugin:core) '!version' invoked");
    std::string version(NAME);
    version += " v";
    version += VERSION;
    client.respond(message, version);
}

void handle_source(irc::client &client, const irc::message &message, void *)
{
    print_log("(plugin:core) '!source' invoked");
    client.respond(message, GITHUB_URL);
}
