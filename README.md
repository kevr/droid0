droid0
------

An Internet Relay Chat bot with plugin support written in C++14, which is
chosen for its modern features while still remaining compatible with older
releases.

## Meson build options

* `version (default: 0.0.1)`: Provide a version to the build
    * Tip: retrieve tags using `git pull --tags` before using `git describe`
    * `meson setup -Dversion="$(git describe)" build`
* `tests (default: true)`: Build test executables
    * `meson setup -Dtests=false build`
* `executable (default: true)`: Build the droid0 executable
    * `meson setup -Dexecutable=false build`

## State

At the moment, this project is just starting and is not ready for external
contributions. Once the project releases version 1.0.0, open contributions
will be opened and welcomed by anybody making use of this software.

## Licensing

This project operates under the MIT public license, kept in hard copy in the
project root at [LICENSE](https://github.com/kevr/droid0/tree/master/LICENSE).
