/**
 * Temporary directory utilities for testing purposes.
 *
 * SPDX-License-Identifier: MIT
 **/
#include "tmpdir.hpp"
#include <gtest/gtest.h>

using namespace droid0;

class tmpdir_test : public testing::Test
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

TEST(tmpdir, rmtree_unable_to_open)
{
    EXPECT_THROW(testutil::rmtree("/unable_to_open"), std::logic_error);
}

TEST_F(tmpdir_test, rmtree_recursive)
{
    std::string nested = tmpdir + "/nested";
    mkdir(nested.c_str(), S_IWUSR | S_IRUSR | S_IXUSR);

    std::string _ = nested + "/nested";
    mkdir(_.c_str(), S_IWUSR | S_IRUSR | S_IXUSR);

    EXPECT_NO_THROW(testutil::rmtree(nested));
}
