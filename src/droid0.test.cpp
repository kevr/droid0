/**
 * Unit test for the program's main entrypoint.
 *
 * SPDX-License-Identifier: MIT
 **/
#define main main_
#include "droid0.cpp"
#undef main

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

TEST(droid0, main)
{
    int argc = 1;
    const char *argv[] = {"droid0"};
    EXPECT_EQ(main_(argc, argv), 0);
}

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

TEST(droid0, verbosity)
{
    int argc = 2;
    const char *argv[] = {"droid0", "-v3"};
    CaptureStdout();
    EXPECT_EQ(main_(argc, argv), 0);
    auto output = GetCapturedStdout();
    EXPECT_NE(output.find("set verbosity level to 3"), std::string::npos);
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

TEST_F(main_test, config)
{
    std::string configfile = tmpdir + "/test.conf";
    std::ofstream ofs(configfile.c_str(), std::ios::out);
    ofs << "verbosity = 3\n";
    ofs.close();

    int argc = 3;
    const char *argv[] = {"droid0", "--config", configfile.c_str()};
    CaptureStdout();
    EXPECT_EQ(main_(argc, argv), 0);
    auto output = GetCapturedStdout();
    EXPECT_NE(output.find("set verbosity level to 3"), std::string::npos);
}

TEST_F(main_test, logging)
{
    std::string logfile = tmpdir + "/test.log";
    int argc = 3;
    const char *argv[] = {"droid0", "--log", logfile.c_str()};
    EXPECT_EQ(main_(argc, argv), 0);
}
