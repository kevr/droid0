/**
 * Boost.Asio TCP server
 *
 * SPDX-License-Identifier: MIT
 **/
#ifndef NET_SERVER_HPP
#define NET_SERVER_HPP

#include "connection.hpp"
#include <boost/asio.hpp>
#include <functional>
#include <memory>

namespace droid0
{
namespace tcp
{

class server
{
  public:
    using accept_func_t =
        std::function<void(server &, std::shared_ptr<connection>)>;

  private:
    bool m_stopped = false;
    boost::asio::io_context &m_io;
    boost::asio::ip::tcp::acceptor m_acceptor;

    //! A successful connection accept callback
    static const accept_func_t default_accept;
    accept_func_t m_on_accept = default_accept;

    //! A successful connection readline callback
    static const connection::readline_func_t default_readline;
    connection::readline_func_t m_on_readline = default_readline;

    static const connection::close_func_t default_close;
    connection::close_func_t m_on_close = default_close;

  public:
    //! Construct a TCP server
    server(boost::asio::io_context &io, short port);

    //! Start listening on IPv4:port
    server &start();

    //! Set server's accept callback
    server &on_accept(accept_func_t accept_handler = default_accept);

    //! Set new connection's readline callback
    server &on_readline(
        connection::readline_func_t readline_handler = default_readline);

    //! Set a new connection's close callback
    server &on_close(connection::close_func_t close_handler = default_close);

    //! Stop accepting connections
    void stop();

  private:
    void start_accept();
    void handle_accept(std::shared_ptr<connection> new_connection,
                       const boost::system::error_code &ec);
};

}; // namespace tcp
}; // namespace droid0

#endif /* NET_SERVER_HPP */
