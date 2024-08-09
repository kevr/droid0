/**
 * Unit test for the program's main entrypoint.
 *
 * SPDX-License-Identifier: MIT
 **/
#define main main_
#include "droid0.cpp"
#undef main

#include "config.hpp"
#include "string.hpp"
#include <gtest/gtest.h>
#include <testutil/tmpdir.hpp>

using testing::internal::CaptureStdout;
using testing::internal::GetCapturedStdout;

class main_test : public testing::Test
{
  protected:
    std::string tmpdir;

  public:
    void SetUp() override
    {
        tmpdir = testutil::mkdtemp();
    }

    void TearDown() override
    {
        testutil::rmtree(tmpdir);
    }
};

TEST(droid0, unknown_option)
{
    int argc = 2;
    const char *argv[] = {"droid0", "--fake"};
    EXPECT_EQ(main_(argc, argv), 1);
}

TEST(droid0, invalid_verbosity)
{
    int argc = 2;
    const char *argv[] = {"droid0", "-v5"};
    EXPECT_EQ(main_(argc, argv), 2);
}

TEST(droid0, help)
{
    int argc = 2;
    const char *argv[] = {"droid0", "--help"};
    EXPECT_EQ(main_(argc, argv), 0);
}

TEST(droid0, version)
{
    int argc = 2;
    const char *argv[] = {"droid0", "--version"};
    CaptureStdout();
    EXPECT_EQ(main_(argc, argv), 0);
    auto output = GetCapturedStdout();
    EXPECT_EQ(rstrip(output), VERSION);
}
