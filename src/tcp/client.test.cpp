/**
 * Unit tests for tcp::client
 *
 * SPDX-License-Identifier: MIT
 **/
#include "client.hpp"
#include "../logger.hpp"
#include "resolve.hpp"
#include "server.hpp"
#include "gtest/gtest.h"
#include <gtest/gtest.h>

using namespace droid0;

class client_test : public testing::Test
{
  protected:
    const std::string host = "127.0.0.1";
    const std::string port = "31337";

    boost::asio::io_service m_io;
    tcp::server m_server{m_io, static_cast<short>(std::stoi(port))};
    tcp::client m_client{m_io};

  public:
    void SetUp() override
    {
        logging.set_loglevel(loglevel::trace);
    }

    void TearDown() override
    {
        logging.set_loglevel();
    }
};

TEST_F(client_test, connects)
{
    m_server
        .on_accept([&](tcp::server &, std::shared_ptr<tcp::connection> conn) {
            conn->stop();
        })
        .on_close([this](auto, auto) { m_server.stop(); })
        .start();

    auto results = tcp::resolve(m_io, host.c_str(), port.c_str());
    m_client
        .on_connect(
            [](tcp::client &client, const boost::system::error_code &ec, auto) {
                EXPECT_FALSE(ec);
                client.stop();
            })
        .start(results);

    m_io.run();
}

TEST_F(client_test, server_ping_client_pong)
{
    // On connection accept, send a "PING\n" message
    m_server
        // When a client connects, send a "PING\n" message to it
        .on_accept([](tcp::server &, auto conn) { conn->write("PING\n"); })
        // When the connection receives a message line, stop the connection
        // and exercise a connection's failure to write due to closed socket
        .on_readline([](std::shared_ptr<tcp::connection> conn,
                        const boost::system::error_code &,
                        const std::string &line) {
            EXPECT_EQ(line, "PONG");
            conn->stop();
            EXPECT_NO_THROW(conn->write("test"));
        })
        .start();

    auto results = tcp::resolve(m_io, host.c_str(), port.c_str());
    m_client
        // On client "PING\n" retrieval, send "PONG\n"
        .on_readline([](tcp::client &client, const boost::system::error_code &,
                        std::string line) {
            EXPECT_EQ(line, "PING");
            client.write("PONG\n");
        })
        // On socket closure, try writing to the client to exercise
        // the write failure path, then stop the server's acceptor
        .on_close([this](tcp::client &client, auto) {
            EXPECT_NO_THROW(client.write("test"));
            m_server.stop();
        })
        .set_name("test")
        .start(results);

    m_io.run();
}

TEST_F(client_test, client_ping_server_pong)
{
    m_server
        .on_readline([](std::shared_ptr<tcp::connection> conn,
                        const boost::system::error_code &,
                        const std::string &line) {
            EXPECT_EQ(line, "PING");
            EXPECT_NO_THROW(conn->write("PONG\n"));
        })
        .on_close(
            [this](std::shared_ptr<tcp::connection>, auto) { m_server.stop(); })
        .start();

    auto results = tcp::resolve(m_io, host.c_str(), port.c_str());
    m_client
        .on_connect(
            [](tcp::client &client, const boost::system::error_code &ec, auto) {
                EXPECT_FALSE(ec);
                client.write("PING\n");
            })
        .on_readline([](tcp::client &client,
                        const boost::system::error_code &ec,
                        const std::string &line) {
            EXPECT_FALSE(ec);
            EXPECT_EQ(line, "PONG");
            client.stop();
        })
        .on_close([](tcp::client &client, auto) {
            EXPECT_NO_THROW(client.write("test"));
        })
        .start(results);

    m_io.run();
}

TEST_F(client_test, default_connection_readline)
{
    m_server.start();

    auto results = tcp::resolve(m_io, host.c_str(), port.c_str());
    m_client
        .on_connect(
            [](tcp::client &client, auto, auto) { client.write("test\n"); })
        .start(results);

    EXPECT_THROW(m_io.run(), std::logic_error);
}

TEST_F(client_test, default_client_readline)
{
    m_server.on_accept([](tcp::server &, auto conn) { conn->write("test\n"); });
    m_server.start();

    auto results = tcp::resolve(m_io, host.c_str(), port.c_str());
    m_client.start(results);

    EXPECT_THROW(m_io.run(), std::logic_error);
}

TEST_F(client_test, fail_to_connect)
{
    std::string port("6666");
    auto results = tcp::resolve(m_io, host.c_str(), port.c_str());
    m_client.start(results);

    m_io.run();
}
