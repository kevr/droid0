/**
 * Boost.Asio-based asynchronous TCP client class
 *
 * SPDX-License-Identifier: MIT
 **/
#ifndef NET_CLIENT_HPP
#define NET_CLIENT_HPP

#include "handlers.hpp"
#include <boost/asio.hpp>

namespace droid0
{
namespace tcp
{

class client
{
  private:
    bool m_stopped;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::streambuf m_input_buffer;

    //! Readline handler
    using read_func_t = readline_function<client &>;
    static const read_func_t default_readline;
    read_func_t m_on_readline = default_readline;

    //! Connection handler
    using connect_func_t =
        std::function<void(client &, const boost::system::error_code &,
                           boost::asio::ip::tcp::resolver::iterator)>;
    static const connect_func_t default_connect;
    connect_func_t m_on_connect = default_connect;

    //! Closure handler
    using close_func_t = close_function<client &>;
    static const close_func_t default_close;
    close_func_t m_on_close = default_close;

  public:
    //! Construct with an io_service, but don't start the client yet
    client(boost::asio::io_service &io);

    //! Set a new connect handler (omit argument ot set default)
    client &on_connect(connect_func_t handle_connect = default_connect);

    //! Set a new readline handler (omit argument to set default)
    client &on_readline(read_func_t handle_readline = default_readline);

    //! Set a new on socket close handler
    client &on_close(close_func_t handle_close = default_close);

    //! Start the client with an external endpoint iterator
    void start(boost::asio::ip::tcp::resolver::iterator endpoint_iter);

    //! Write data to the socket
    void write(std::string data);

    //! Stop the client
    void stop();

  private:
    //! Check and react to an expired deadline
    void check_deadline();

    //! Start asynchronous TCP connection actor
    void start_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iter);

    //! A callback which handles an async connection actor's result
    void handle_connect(const boost::system::error_code &ec,
                        boost::asio::ip::tcp::resolver::iterator endpoint_iter);

    //! Start asynchronous TCP readline actor
    void start_readline();

    //! A callback which handles an async readline actor's result
    void handle_readline(const boost::system::error_code &ec);

    //! A callback whichhandles an async write actor's result
    void handle_write(const boost::system::error_code &ec, std::string message);
};

}; // namespace tcp
}; // namespace droid0

#endif /* NET_CLIENT_HPP */
