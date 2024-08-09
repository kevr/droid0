// SPDX-License-Identifier: MIT
#ifndef PLUGIN_PLUGIN_HPP
#define PLUGIN_PLUGIN_HPP

#include "subscription.hpp"

extern "C" void
droid0_bootstrap(droid0::plugin::subscriptions<std::string> &subs);

#endif /* PLUGIN_PLUGIN_HPP */
