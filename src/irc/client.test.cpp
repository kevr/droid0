#include "client.hpp"
#include "../logger.hpp"
#include "../string.hpp"
#include "../tcp/resolve.hpp"
#include "../tcp/server.hpp"
#include <gtest/gtest.h>

using namespace droid0;

class irc_client_test : public testing::Test
{
  protected:
    const std::string host = "127.0.0.1";
    const std::string port = "31337";

    boost::asio::io_service io;
    tcp::server m_server{io, static_cast<short>(std::stoi(port))};
    irc::client m_client{io, "test", "test", "Test User"};

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

TEST_F(irc_client_test, works)
{
    using func_t = std::function<void(std::shared_ptr<tcp::connection>,
                                      const boost::system::error_code &,
                                      const std::string &)>;
    std::vector<func_t> server_readline{
        [](auto, auto &, auto &line) {
            EXPECT_EQ(line, "NICK test");
        },
        [](auto conn, auto &, auto &line) {
            EXPECT_EQ(line, "USER test * * :Test User");
            conn->write(":irc.example.org 001 blah\n");
        },
    };

    int server_i = 0;
    m_server.on_readline([&](std::shared_ptr<tcp::connection> a,
                             const boost::system::error_code &b,
                             const std::string &c) {
        server_readline[server_i++](a, b, c);
    });
    m_server.start();

    using client_func_t =
        std::function<void(irc::client &, const irc::message &)>;
    std::vector<client_func_t> client_readline{
        [this](auto &c, const irc::message &message) {
            EXPECT_EQ(message.elements().at(1), "001");
            m_server.stop();
            c.stop();
        },
    };

    int client_i = 0;
    m_client.on_readline([&](irc::client &a, const irc::message &b) {
        client_readline[client_i++](a, b);
    });

    auto results = tcp::resolve(io, host, port);
    m_client.start(results);

    io.run();
}
