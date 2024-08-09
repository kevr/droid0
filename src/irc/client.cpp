// SPDX-License-Identifier: MIT
#include "client.hpp"
#include "../logger.hpp"
#include "../plugin/manager.hpp"
#include "../string.hpp"
#include "message.hpp"
#include "opcode.hpp"
#include "string.hpp"
#include <fmt/format.h>

using namespace std::placeholders;

namespace droid0::irc
{

client::client(boost::asio::io_service &io, const std::string &nick_,
               const std::string &user_, const std::string &realname_)
    : tcp::client(io)
{
    init_callbacks();

    // Set  client name for logs
    set_name(nick_);

    nick(nick_);
    user(user_);
    realname(realname_);

    m_plugin_mgr = new plugin::manager;
}

client::~client()
{
    auto *mgr = reinterpret_cast<plugin::manager *>(m_plugin_mgr);
    mgr->unload();
    delete mgr;
}

void client::start(boost::asio::ip::tcp::resolver::iterator ep)
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

const std::string &client::nick(const std::string &nick_)
{
    m_nick = nick_;
    return nick();
}

const std::string &client::nick() const
{
    return m_nick;
}

const std::string &client::user(const std::string &user_)
{
    m_user = user_;
    return user();
}

const std::string &client::user() const
{
    return m_user;
}

const std::string &client::realname(const std::string &realname_)
{
    m_realname = realname_;
    return realname();
}

const std::string &client::realname() const
{
    return m_realname;
}

std::vector<std::string> &client::channels()
{
    return m_channels;
}

void client::on_connect(connect_func_t f)
{
    m_on_connect = f;
}

void client::on_readline(readline_func_t f)
{
    m_on_readline = f;
}

void client::bootstrap(const conf::config &cfg, const irc::config &network)
{
    // Adopt the configured prefix (default '!')
    m_prefix = network.prefix();
    if (std::isalpha(m_prefix.front())) {
        m_prefix.push_back(' ');
    }

    // Load and bootstrap network plugins
    *m_plugin_mgr = plugin::manager(cfg, network);
    m_plugin_mgr->load();
}

void client::join(const std::string &channel)
{
    write(string::join(channel));
}

void client::privmsg(const std::string &dest, const std::string &text)
{
    write(string::privmsg(dest, text));
}

void client::respond(const irc::message &orig, const std::string &text)
{
    const auto &dest = orig.elements().at(2);
    privmsg(dest, fmt::format("{}: {}", orig.src().nick(), text));
}

void client::init_callbacks()
{
    m_callback[to_string(opcode::connected)] =
        std::bind(&client::perform, this, _1, _2, _3);
    m_callback[to_string(opcode::joined)] =
        std::bind(&client::handle_join, this, _1, _2, _3);
    m_callback["PING"] = std::bind(&client::handle_ping, this, _1, _2, _3);
    m_callback["PRIVMSG"] =
        std::bind(&client::handle_privmsg, this, _1, _2, _3);
}

void client::handle_readline(const std::string &line)
{
    irc::message message(line);
    if (auto it = m_callback.find(message.src()); it != m_callback.end()) {
        // If the message source is a callback, prioritize it
        it->second(*this, message, nullptr);
    } else if (auto it = m_callback.find(message.elements().at(1));
               it != m_callback.end()) {
        // Fallback to running a command if it's a callback
        it->second(*this, message, nullptr);
    }

    m_on_readline(*this, message);
}

void client::handle_ping(client &, irc::message &message, void *)
{
    write(string::pong(message.arg().value()));
}

void client::handle_join(client &, irc::message &message, void *)
{
    auto nick = message.elements().at(2);
    auto ch = message.elements().at(4);
    auto names = droid0::join(split(message.arg().value()), ", ");
    logging.info(fmt::format("{} joined {}, users: {}", nick, ch, names));
}

void client::handle_privmsg(client &client, irc::message &message, void *data)
{
    message.parse_command(m_prefix);
    if (message.command().empty())
        return;

    auto source = message.src().substr(1);

    logging.info(fmt::format("({}) {} <{}> {}", nick(),
                             message.elements().at(2), source,
                             message.arg().value()));

    if (m_plugin_mgr != nullptr) {
        m_plugin_mgr->call(message.command(), client, message, data);
    }
}

void client::perform(client &, irc::message &, void *)
{
    // Autojoin channels
    join(droid0::join(m_channels, ","));
}

}; // namespace droid0::irc
