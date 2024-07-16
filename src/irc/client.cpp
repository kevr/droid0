// SPDX-License-Identifier: MIT
#include "client.hpp"
#include "../logger.hpp"
#include "../string.hpp"
#include "message.hpp"
#include "string.hpp"

using namespace std::placeholders;

namespace droid0
{

irc::client::client(boost::asio::io_service &io, const std::string &nick_,
                    const std::string &user_, const std::string &realname_)
    : tcp::client(io)
{
    init_callbacks();

    // Set  client name for logs
    set_name(nick_);

    nick(nick_);
    user(user_);
    realname(realname_);
}

void irc::client::start(boost::asio::ip::tcp::resolver::iterator ep)
{
    tcp::client::on_connect([this](tcp::client &,
                                   const boost::system::error_code &error,
                                   boost::asio::ip::tcp::resolver::iterator) {
        if (error) {
            return logging.error(error.message());
        }
        write(string::registration(m_nick, m_user, m_realname));
        m_on_connect(*this);
    });
    tcp::client::on_readline([this](tcp::client &,
                                    const boost::system::error_code &,
                                    const std::string &line) {
        // Handle readline
        handle_readline(line);
    });

    tcp::client::start(ep);
}

const std::string &irc::client::nick(const std::string &nick_)
{
    m_nick = nick_;
    return nick();
}

const std::string &irc::client::nick() const
{
    return m_nick;
}

const std::string &irc::client::user(const std::string &user_)
{
    m_user = user_;
    return user();
}

const std::string &irc::client::user() const
{
    return m_user;
}

const std::string &irc::client::realname(const std::string &realname_)
{
    m_realname = realname_;
    return realname();
}

const std::string &irc::client::realname() const
{
    return m_realname;
}

std::vector<std::string> &irc::client::channels()
{
    return m_channels;
}

void irc::client::on_connect(connect_func_t f)
{
    m_on_connect = f;
}

void irc::client::on_readline(readline_func_t f)
{
    m_on_readline = f;
}

void irc::client::init_callbacks()
{
    m_callback["001"] = std::bind(&client::perform, this, _1, _2, _3);
    m_callback["353"] = std::bind(&client::handle_join, this, _1, _2, _3);
    m_callback["PING"] = std::bind(&client::handle_ping, this, _1, _2, _3);
    m_callback["PRIVMSG"] =
        std::bind(&client::handle_privmsg, this, _1, _2, _3);
}

void irc::client::handle_readline(const std::string &line)
{
    irc::message message(line);
    if (auto it = m_callback.find(message.source()); it != m_callback.end()) {
        // If the message source is a callback, prioritize it
        it->second(*this, message, nullptr);
    } else if (auto it = m_callback.find(message.elements().at(1));
               it != m_callback.end()) {
        // Fallback to running a command if it's a callback
        it->second(*this, message, nullptr);
    }

    m_on_readline(*this, message);
}

void irc::client::handle_ping(irc::client &, const irc::message &message,
                              void *)
{
    write(string::pong(message.arg().value()));
}

void irc::client::handle_join(irc::client &, const irc::message &message,
                              void *)
{
    auto nick = message.elements().at(2);
    auto ch = message.elements().at(4);
    auto names = join(split(message.arg().value()), ", ");
    std::stringstream ss;
    ss << nick << " joined " << ch << ", users: " << names;
    logging.info(ss.str());
}

void irc::client::handle_privmsg(irc::client &, const irc::message &message,
                                 void *)
{
    auto source = message.source().substr(1);

    std::stringstream ss;
    ss << '(' << nick() << ") " << message.elements().at(2) << " <" << source
       << "> " << message.arg().value();
    logging.info(ss.str());
}

void irc::client::perform(irc::client &, const irc::message &, void *)
{
    // Autojoin channels
    for (auto &channel : m_channels) {
        write(string::join(channel));
    }
}

}; // namespace droid0
