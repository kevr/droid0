droid0
------

An Internet Relay Chat bot with plugin support written in C++17, which is
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

## Plugins

See [src/plugins/meson.build](src/plugins/meson.build) and
[src/plugins/echo.cpp](src/plugins/echo.cpp) for a plugin example.

To add your own plugin, copy echo's strategy by adding a name.cpp file to
`src/plugins` and compiling it into a library in `src/plugins/meson.build`.

## State

At the moment, this project is just starting and is not ready for external
contributions. Once the project releases version 1.0.0, open contributions
will be opened and welcomed by anybody making use of this software.

### Current Goals

- [x] Basic IRC client
- [x] Basic plugin functionality
    - [src/plugins/core.cpp](src/plugins/core.cpp)
    - [src/plugins/echo.cpp](src/plugins/echo.cpp)
- [ ] Advanced IRC client; support all needed IRC features
    - NOTICE, WHOIS, etc
- [ ] Access control list
- [ ] Cover everything with tests
- [ ] Web search plugin

## Licensing

This project operates under the MIT public license, kept in hard copy in the
project root at [LICENSE](https://github.com/kevr/droid0/tree/master/LICENSE).
