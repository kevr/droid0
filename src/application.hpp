// SPDX-License-Identifier: MIT
#ifndef SRC_APPLICATION_HPP
#define SRC_APPLICATION_HPP

#include "conf/config.hpp"

namespace droid0
{

class application
{
  private:
    conf::config config;

  public:
    int run(int argc, const char *argv[]);

  private:
    int parse_config(int argc, const char *argv[]);
    bool set_verbosity();
};

}; // namespace droid0

#endif /* SRC_APPLICATION_HPP */
