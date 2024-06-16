/**
 * Temporary directory utilities for testing purposes.
 *
 * SPDX-License-Identifier: MIT
 **/
#ifndef TESTING_TMPDIR_HPP
#define TESTING_TMPDIR_HPP

#include <string>

namespace droid0
{
namespace testutil
{

std::string mkdtemp();
void rmtree(const std::string &path);

}; // namespace testutil
}; // namespace droid0

#endif /* TESTING_TMPDIR_HPP */
