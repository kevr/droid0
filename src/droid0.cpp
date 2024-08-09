/**
 * Main entrypoint for the droid0 executable.
 *
 * SPDX-License-Identifier: MIT
 **/
#include "application.hpp"

using namespace droid0;

int main(int argc, const char **argv)
{
    application app;
    return app.run(argc, argv);
}
