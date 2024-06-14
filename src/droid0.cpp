/**
 * Main entrypoint for the droid0 executable.
 *
 * SPDX-License-Identifier: MIT
 **/
#include <iostream>
#include "config.hpp"

int main(int argc, char **argv) {
    if(argc != 1) {
        std::cout << argv[0] <<  "takes no arguments.\n";
        return 1;
    }
    std::cout << "This is project " << NAME << ", version " << VERSION << ".\n";
    return 0;
}
