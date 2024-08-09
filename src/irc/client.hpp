// SPDX-License-Identifier: MIT
#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include "../conf/config.hpp"
#include "../tcp/client.hpp"
#include "config.hpp"
#include "message.hpp"
#include <functional>
#include <unordered_map>

namespace droid0::plugin
{
class manager;
};

namespace droid0::irc
{

class client : public tcp::client
{
  private:
    std::string m_prefix = "!";

    // Client information
    std::string m_nick;
    std::string m_user;
    std::string m_realname;

    // Metadata
    std::vector<std::string> m_channels;

    // Callbacks
    using callback_func_t =
        std::function<void(irc::client &, irc::message &, void *)>;
    std::unordered_map<std::string, callback_func_t> m_callback;

    // Connect callback
    using connect_func_t = std::function<void(irc::client &)>;
    connect_func_t m_on_connect = [](irc::client &) {};

    // Readline callback
    using readline_func_t =
        std::function<void(irc::client &, const irc::message &)>;
    readline_func_t m_on_readline = [](irc::client &, const irc::message &) {};

    // Pointer to plugin::manager; type erased here to avoid cyclic
    plugin::manager *m_plugin_mgr = nullptr;

  public:
    //! Value constructor
    client(boost::asio::io_service &io, const std::string &nick,
           const std::string &user, const std::string &realname);

    //! Destructor
    ~client();

  public:
    //! Start the client
    void start(boost::asio::ip::tcp::resolver::iterator ep);

    //! Set nick
    const std::string &nick(const std::string &nick_);

    //! Return internal nick
    const std::string &nick() const;

    //! Set user
    const std::string &user(const std::string &user_);

    //! Return internal user
    const std::string &user() const;

    //! Set realname
    const std::string &realname(const std::string &realname_);

    //! Return internal realname
    const std::string &realname() const;

    //! Channels split into a vector of single channels
    std::vector<std::string> &channels();

    //! Configure external on_connect callback
    void on_connect(connect_func_t f);

    //! Configure external on_readline callback
    void on_readline(readline_func_t f);

    //! Bootstrap the client: deserialize config, load plugins, etc
    void bootstrap(const conf::config &cfg, const irc::config &network);

    //! Join a channel
    void join(const std::string &channel);

    //! Send a PRIVMSG to `dest`
    void privmsg(const std::string &dest, const std::string &message);

    //! Respond to `orig`'s source nick with `text`
    void respond(const irc::message &orig, const std::string &text);

  private:
    // Initialize internal callbacks
    void init_callbacks();

    // Handle a single readline
    void handle_readline(const std::string &line);

    // Handle PING
    void handle_ping(irc::client &, irc::message &, void *);

    // Handle JOIN
    void handle_join(irc::client &, irc::message &, void *);

    // Handle PRIVMSG
    void handle_privmsg(irc::client &, irc::message &, void *);

    // Automated on-registration actions
    void perform(irc::client &, irc::message &, void *);
};

}; // namespace droid0::irc

#endif /* IRC_CLIENT_HPP */
