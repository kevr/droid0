/**
 * Boost.Asio TCP server connection
 *
 * SPDX-License-Identifier: MIT
 **/
#ifndef NET_CONNECTION_HPP
#define NET_CONNECTION_HPP

#include "handlers.hpp"
#include <boost/asio.hpp>

namespace droid0
{
namespace tcp
{

class connection : public std::enable_shared_from_this<connection>
{
  public:
    using readline_func_t = readline_function<std::shared_ptr<connection>>;
    readline_func_t m_on_readline;

    using close_func_t = close_function<std::shared_ptr<connection>>;
    close_func_t m_on_close;

  private:
    bool m_stopped = false;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::streambuf m_input_buffer;

  private: // private friends
    friend class server;

  public:
    static std::shared_ptr<connection> create(boost::asio::io_context &io,
                                              readline_func_t readline_handler,
                                              close_func_t close_handler);

  public:
    void start();
    void stop();
    void write(const std::string &data);

  private:
    connection(boost::asio::io_context &io, readline_func_t readline_handler,
               close_func_t close_handler);

  private:
    void start_readline();
    void handle_readline(const boost::system::error_code &ec);
    void handle_write(const boost::system::error_code &ec, std::string message);
};

}; // namespace tcp
}; // namespace droid0

#endif /* NET_CONNECTION_HPP */
