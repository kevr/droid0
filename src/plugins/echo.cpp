#include "../irc/client.hpp"
#include "../irc/message.hpp"
#include "../logger.hpp"
#include "../plugin/plugin.hpp"

using namespace droid0;

// Plugin code
void print_log(const std::string &message);
void handle_echo(irc::client &client, const irc::message &message, void *);

void droid0_bootstrap(droid0::plugin::subscriptions<std::string> &subs)
{
    subs.subscribe("echo", handle_echo);
}

void print_log(const std::string &message)
{
    logging.info("(plugin:echo) " + message);
}

void handle_echo(irc::client &client, const irc::message &message, void *)
{
    print_log("(plugin:core) '!echo' invoked");
    client.privmsg(message.elements().at(2), message.command_arg());
}
