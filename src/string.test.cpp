#include "string.hpp"
#include <gtest/gtest.h>

using namespace droid0;

TEST(string, lstrip)
{
    EXPECT_EQ(lstrip(" test"), "test");
}

TEST(string, rstrip)
{
    EXPECT_EQ(rstrip("test\n"), "test");
}

TEST(string, strip)
{
    EXPECT_EQ(strip("  test \n"), "test");
}
