/**
 * Unit test for the program's main entrypoint.
 *
 * SPDX-License-Identifier: MIT
 **/
#define main main_
#include "droid0.cpp"
#undef main

#include <gtest/gtest.h>

using testing::internal::CaptureStdout;
using testing::internal::GetCapturedStdout;

TEST(droid0, main)
{
    int argc = 1;
    const char *argv[] = {"droid0"};

    CaptureStdout();
    EXPECT_EQ(main_(argc, argv), 0);
    auto output = GetCapturedStdout();

    std::string expected("This is project droid0, version ");
    expected.append(VERSION);
    expected.append(".\n");
    EXPECT_EQ(output, expected);
}

TEST(droid0, no_arguments)
{
    int argc = 2;
    const char *argv[] = {"droid0", "blah"};

    CaptureStdout();
    EXPECT_EQ(main_(argc, argv), 1);
    auto output = GetCapturedStdout();
    EXPECT_EQ(output, "droid0 takes no arguments.\n");
}
