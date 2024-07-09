/**
 * Boost.Asio-based asynchronous TCP client class
 *
 * SPDX-License-Identifier: MIT
 **/
#include "client.hpp"
#include "../logger.hpp"
#include "../string.hpp"
#include "errors.hpp"
#include <functional>
#include <iostream>
#include <stdexcept>

using tcp = boost::asio::ip::tcp;
using namespace std::placeholders;

namespace droid0
{
namespace tcp
{

//! Throws std::logic_error; put in place to avoid a dead function call
const client::read_func_t client::default_readline =
    [](client &, const boost::system::error_code &, const std::string &) {
        throw std::logic_error("client::on_readline must be set");
    };

const client::connect_func_t client::default_connect =
    [](client &, const boost::system::error_code &error,
       boost::asio::ip::tcp::resolver::iterator endpoint_iter) {
        if (!error) {
            std::stringstream ss;
            ss << "[Client] connected to " << endpoint_iter->endpoint();
            logging.debug(ss.str());
        } else {
            logging.error("[Client] unable to connect");
        }
    };

const client::close_func_t client::default_close =
    [](client &, const boost::system::error_code &) {};

client::client(boost::asio::io_service &io)
    : m_stopped(false)
    , m_socket(io)
{
}

client &client::on_connect(client::connect_func_t handle_connect)
{
    m_on_connect = handle_connect;
    return *this;
}

client &client::on_readline(client::read_func_t handle_readline)
{
    m_on_readline = handle_readline;
    return *this;
}

client &client::on_close(client::close_func_t handle_close)
{
    m_on_close = handle_close;
    return *this;
}

void client::start(boost::asio::ip::tcp::resolver::iterator endpoint_iter)
{
    start_connect(endpoint_iter);
}

void client::write(std::string data)
{
    boost::asio::async_write(m_socket, boost::asio::buffer(data),
                             std::bind(&client::handle_write, this, _1, data));
}

void client::stop()
{
    SCOPE_TRACE();

    bool closed = !m_stopped;
    m_stopped = true;

    boost::system::error_code error;
    error = m_socket.close(error);

    if (closed) {
        m_on_close(*this, error);
    }
}

void client::start_connect(
    boost::asio::ip::tcp::resolver::iterator endpoint_iter)
{
    SCOPE_TRACE();

    if (endpoint_iter != boost::asio::ip::tcp::resolver::iterator()) {
        std::stringstream ss;
        ss << "connecting to " << endpoint_iter->endpoint() << "...";
        logging.info(ss.str());

        // Begin the async connection chain
        m_socket.async_connect(
            endpoint_iter->endpoint(),
            std::bind(&client::handle_connect, this, _1, endpoint_iter));
    } else {
        stop();
        m_on_connect(*this, boost::asio::error::connection_aborted,
                     endpoint_iter);
    }
}

void client::handle_connect(
    const boost::system::error_code &ec,
    boost::asio::ip::tcp::resolver::iterator endpoint_iter)
{
    SCOPE_TRACE();

    if (endpoint_iter == boost::asio::ip::tcp::resolver::iterator()) {
        m_on_connect(*this, ec, endpoint_iter);
    } else if (ec) {
        // Socket encountered an error
        logging.error("encountered '" + ec.message() + "'");
        m_socket.close();
        start_connect(++endpoint_iter);
    } else {
        // Socket connected
        std::stringstream ss;
        ss << "connected to " << endpoint_iter->endpoint();
        logging.info(ss.str());
        start_readline();
        m_on_connect(*this, ec, endpoint_iter);
    }
}

void client::start_readline()
{
    SCOPE_TRACE();

    if (m_stopped)
        return;

    boost::asio::async_read_until(
        m_socket, m_input_buffer, '\n',
        std::bind(&client::handle_readline, this, _1));
}

void client::handle_readline(const boost::system::error_code &ec)
{
    SCOPE_TRACE();

    if (m_stopped)
        return;

    std::string line;
    std::istream is(&m_input_buffer);
    std::getline(is, line);
    if (!line.empty()) {
        logging.info("[Client:R] " + line);
        m_on_readline(*this, ec, std::move(line));
    }

    if (ec) {
        if (graceful_read_errors.find(ec) != graceful_read_errors.end()) {
            logging.debug("[Client:R] encountered graceful error '" +
                          ec.message() + "'");
        } else {
            logging.error("[Client:R] " + ec.message());
        }

        return stop();
    }

    start_readline();
}

void client::handle_write(const boost::system::error_code &ec,
                          std::string message)
{
    // Otherwise, an error occurred, log it out and stop the socket
    if (ec) {
        if (graceful_write_errors.find(ec) != graceful_write_errors.end()) {
            logging.debug("[Client:L] encountered graceful error '" +
                          ec.message() + "'");
        } else {
            logging.error("[Client:L] " + ec.message());
        }
        stop();
    } else {
        logging.info("[Client:L] " + rstrip(message));
    }
}

}; // namespace tcp
}; // namespace droid0
