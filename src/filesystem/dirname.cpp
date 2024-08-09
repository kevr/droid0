// SPDX-License-Identifier: MIT
#include "dirname.hpp"
#include <libgen.h>

namespace droid0::filesystem
{

std::string dirname(std::string dest)
{
    ::dirname(dest.data());
    return dest;
}

}; // namespace droid0::filesystem
