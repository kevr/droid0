// SPDX-License-Identifier: MIT
#include "stat.hpp"
#include <gtest/gtest.h>

using namespace droid0;

TEST(stat, status)
{
    auto [st, rc] = filesystem::status("/usr");
    EXPECT_NE(rc, -1);
    EXPECT_TRUE(S_ISDIR(st.st_mode));
}

TEST(stat, exists)
{
    EXPECT_TRUE(filesystem::exists("/usr"));
}

TEST(stat, not_exists)
{
    EXPECT_FALSE(filesystem::exists("blah/blah/blah"));
}

TEST(stat, is_directory)
{
    EXPECT_TRUE(filesystem::is_directory("/usr"));
}

TEST(stat, not_is_directory)
{
    EXPECT_FALSE(filesystem::is_directory("blah/blah/blah"));
}
