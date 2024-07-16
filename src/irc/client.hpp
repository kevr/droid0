// SPDX-License-Identifier: MIT
#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include "../tcp/client.hpp"
#include "message.hpp"
#include <functional>
#include <unordered_map>

namespace droid0::irc
{

class client : public tcp::client
{
  private:
    // Client information
    std::string m_nick;
    std::string m_user;
    std::string m_realname;

    // Metadata
    std::vector<std::string> m_channels;

    using callback_func_t =
        std::function<void(irc::client &, const irc::message &, void *)>;
    std::unordered_map<std::string, callback_func_t> m_callback;

    using connect_func_t = std::function<void(irc::client &)>;
    connect_func_t m_on_connect = [](irc::client &) {};

    using readline_func_t =
        std::function<void(irc::client &, const irc::message &)>;
    readline_func_t m_on_readline = [](irc::client &, const irc::message &) {};

  public:
    client(boost::asio::io_service &io, const std::string &nick,
           const std::string &user, const std::string &realname);

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

    std::vector<std::string> &channels();

    void on_connect(connect_func_t f);
    void on_readline(readline_func_t f);

  private:
    // Initialize internal callbacks
    void init_callbacks();

    // Handle a single readline
    void handle_readline(const std::string &line);

    // Handle PING
    void handle_ping(irc::client &, const irc::message &, void *);

    // Handle JOIN
    void handle_join(irc::client &, const irc::message &, void *);

    // Handle PRIVMSG
    void handle_privmsg(irc::client &, const irc::message &, void *);

    void perform(irc::client &, const irc::message &, void *);
};

}; // namespace droid0::irc

#endif /* IRC_CLIENT_HPP */
