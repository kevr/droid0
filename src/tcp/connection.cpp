/**
 * Boost.Asio TCP server connection
 *
 * SPDX-License-Identifier: MIT
 **/
#include "connection.hpp"
#include "../logger.hpp"
#include "../string.hpp"
#include "errors.hpp"

using namespace std::placeholders;

namespace droid0
{
namespace tcp
{

std::shared_ptr<connection>
connection::create(boost::asio::io_context &io,
                   connection::readline_func_t readline_handler,
                   connection::close_func_t close_handler)
{
    return std::shared_ptr<connection>(
        new connection(io, readline_handler, close_handler));
}

void connection::start()
{
    start_readline();
}

void connection::stop()
{
    SCOPE_TRACE();

    bool closed = !m_stopped;
    m_stopped = true;

    boost::system::error_code error;
    error = m_socket.shutdown(m_socket.shutdown_both, error);
    error = m_socket.close(error);

    if (closed) {
        m_on_close(shared_from_this(), error);
    }
}

void connection::write(const std::string &data)
{
    if (m_stopped)
        return;

    boost::asio::async_write(
        m_socket, boost::asio::buffer(data),
        std::bind(&connection::handle_write, shared_from_this(), _1, data));
}

connection::connection(boost::asio::io_context &io,
                       connection::readline_func_t readline_handler,
                       connection::close_func_t close_handler)
    : m_on_readline(readline_handler)
    , m_on_close(close_handler)
    , m_socket(io)
{
}

void connection::start_readline()
{
    SCOPE_TRACE();

    if (m_stopped)
        return;

    boost::asio::async_read_until(
        m_socket, m_input_buffer, '\n',
        std::bind(&connection::handle_readline, shared_from_this(), _1));
}

void connection::handle_readline(const boost::system::error_code &ec)
{
    SCOPE_TRACE();

    std::string line;
    std::istream is(&m_input_buffer);
    std::getline(is, line);
    if (!line.empty()) {
        logging.info("[Server:R] " + line);
        m_on_readline(shared_from_this(), ec, std::move(line));
    }

    if (ec) {
        if (graceful_read_errors.find(ec) != graceful_read_errors.end()) {
            logging.debug("[Server:R] encountered graceful error '" +
                          ec.message() + "'");
        } else {
            logging.error("[Server:R] " + ec.message());
        }

        return stop();
    }

    start_readline();
}

void connection::handle_write(const boost::system::error_code &ec,
                              std::string message)
{
    if (ec) {
        if (graceful_write_errors.find(ec) != graceful_write_errors.end()) {
            logging.debug("[Server:L] encountered graceful error '" +
                          ec.message() + "'");
        } else {
            logging.error("[Server:L] " + ec.message());
        }
        stop();
    } else {
        logging.info("[Server:L] " + rstrip(message));
    }
}

}; // namespace tcp
}; // namespace droid0
