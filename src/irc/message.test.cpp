#include "message.hpp"
#include <gtest/gtest.h>

using namespace droid0;

TEST(message, front_out_of_range)
{
    EXPECT_THROW(irc::message(""), std::out_of_range);
}

TEST(message, no_argument)
{
    EXPECT_EQ(irc::message(":source cmd params").arg(), std::nullopt);
}

TEST(message, works)
{
    irc::message message(":source cmd param1 param2 :argument text");
    EXPECT_EQ(message.arg().value(), "argument text");
    EXPECT_EQ(message.first(), ":source");
    EXPECT_EQ(message.src(), ":source");
}
