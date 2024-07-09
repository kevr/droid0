/**
 * Boost.Asio TCP server
 *
 * SPDX-License-Identifier: MIT
 **/
#include "server.hpp"
#include "../logger.hpp"

using namespace std::placeholders;

namespace droid0
{
namespace tcp
{

const server::accept_func_t server::default_accept =
    [](server &, std::shared_ptr<connection>) { SCOPE_TRACE(); };

const connection::readline_func_t server::default_readline =
    [](std::shared_ptr<connection>, const boost::system::error_code &,
       const std::string &) {
        throw std::logic_error("server::on_readline must be set");
    };

const connection::close_func_t server::default_close = [](auto, auto) {
    SCOPE_TRACE();
};

server::server(boost::asio::io_context &io, short port)
    : m_io(io)
    , m_acceptor(m_io, boost::asio::ip::tcp::endpoint(
                           boost::asio::ip::tcp::v4(), port))
{
}

server &server::start()
{
    start_accept();
    return *this;
}

server &server::on_accept(server::accept_func_t accept_handler)
{
    m_on_accept = accept_handler;
    return *this;
}

server &server::on_readline(connection::readline_func_t readline_handler)
{
    m_on_readline = readline_handler;
    return *this;
}

server &server::on_close(connection::close_func_t close_handler)
{
    m_on_close = close_handler;
    return *this;
}

void server::stop()
{
    m_stopped = true;
    m_acceptor.cancel();
}

void server::start_accept()
{
    if (m_stopped)
        return;

    std::shared_ptr<connection> new_connection =
        connection::create(m_io, m_on_readline, m_on_close);
    m_acceptor.async_accept(
        new_connection->m_socket,
        std::bind(&server::handle_accept, this, new_connection, _1));
}

void server::handle_accept(std::shared_ptr<connection> new_connection,
                           const boost::system::error_code &ec)
{
    m_on_accept(*this, new_connection);

    if (!ec) {
        new_connection->start();
    }

    start_accept();
}

}; // namespace tcp
}; // namespace droid0
