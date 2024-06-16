/**
 * A unit test for the logger class
 **/
#include "logger.hpp"
#include "string.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <regex>

using namespace droid0;
using testing::internal::CaptureStdout;
using testing::internal::GetCapturedStdout;

static const std::regex log_regex(
    R"(\[[0-9]{4}\-[0-9]{2}\-[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2} .{3}\] \[.*\] .*)");

TEST(logger, info)
{
    CaptureStdout();
    logging.info("test");
    auto output = rstrip(GetCapturedStdout());
    EXPECT_TRUE(std::regex_match(output, log_regex));
}

TEST(logger, warning)
{
    CaptureStdout();
    logging.warning("test");
    auto output = rstrip(GetCapturedStdout());
    EXPECT_TRUE(std::regex_match(output, log_regex));
}

TEST(logger, error)
{
    CaptureStdout();
    logging.error("test");
    auto output = rstrip(GetCapturedStdout());
    EXPECT_TRUE(std::regex_match(output, log_regex));
}

TEST(logger, debug)
{
    // Nothing should be printed with default loglevel
    CaptureStdout();
    logging.debug("test1");
    auto output = GetCapturedStdout();
    EXPECT_EQ(output.size(), 0);

    // But when it's set to debug, something should
    logging.set_loglevel(loglevel::debug);
    CaptureStdout();
    logging.debug("test2");
    output = rstrip(GetCapturedStdout());
    logging.set_loglevel();
    EXPECT_TRUE(std::regex_match(output, log_regex));
}

TEST(logger, trace)
{
    // Nothing should be printed with default loglevel
    CaptureStdout();
    logging.trace("source1", "test1");
    auto output = GetCapturedStdout();
    EXPECT_EQ(output.size(), 0);

    // But when it's set to trace, something should
    logging.set_loglevel(loglevel::trace);
    CaptureStdout();
    logging.trace("source2", "test2");
    output = rstrip(GetCapturedStdout());
    logging.set_loglevel();
    EXPECT_TRUE(std::regex_match(output, log_regex));
}

TEST(logger, set_stream)
{
    logger::set_stream(std::cerr);
    testing::internal::CaptureStderr();
    logging.info("test");
    auto output = rstrip(testing::internal::GetCapturedStderr());
    logger::set_stream();
    EXPECT_TRUE(std::regex_match(output, log_regex));
}
