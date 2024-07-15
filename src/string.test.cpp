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

TEST(string, split)
{
    std::vector<std::string> v = split("a b c");
    std::vector<std::string> expected{"a", "b", "c"};
    EXPECT_EQ(v, expected);
}

TEST(string, join)
{
    std::vector<std::string> v;
    EXPECT_EQ(join(v), "");

    v.emplace_back("1");
    EXPECT_EQ(join(v), "1");

    v.emplace_back("2");
    EXPECT_EQ(join(v), "1,2");
}
