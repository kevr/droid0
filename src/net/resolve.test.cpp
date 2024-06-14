#include "resolve.hpp"
#include <boost/asio.hpp>
#include <gtest/gtest.h>

using tcp = boost::asio::ip::tcp;
using namespace droid0;

class resolve_test : public testing::Test
{
  protected:
    boost::asio::io_service io;
};

TEST_F(resolve_test, localhost_succeeds)
{
    auto retval = net::resolve(io, "127.0.0.1", "80");
    EXPECT_EQ(retval->host_name(), "127.0.0.1");

    EXPECT_EQ(retval->endpoint().address().to_string(), "127.0.0.1");
    EXPECT_EQ(retval->endpoint().port(), 80);
}

TEST_F(resolve_test, unknown_host_throws)
{
    EXPECT_THROW(net::resolve(io, "unknown.host.xyz", "80"), std::exception);
}
