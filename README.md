# C++ syslog wrapper library

The `syslog.h` library, could waste time parsing strings which are not going
to be finally logged. Indeed, `stdarg.h` is used behind so this could make
slower programs when using `syslog` traces.

This library is a simple wrapper static class over `syslog.h` in order to
check the log level in advance and also provides useful macro definitions
to ease messages formatting:

You should protect a whole block of code which only shall be processed when
the appropriate log level is assigned:

>    LOGDEBUG, LOGINFORMATIONAL, LOGNOTICE and LOGWARNING are used for that.

## Integration

[`logger.hpp`](https://github.com/testillano/logger/blob/master/include/ert/tracing/Logger.hpp) is the single required file in `include/ert` or [released here](https://github.com/testillano/logger/releases). You need to add

```cpp
#include <ert/logger.hpp>
```

### CMake

#### Embedded

##### Replication

To embed the library directly into an existing CMake project, place the entire source tree in a subdirectory and call `add_subdirectory()` in your `CMakeLists.txt` file:

```cmake
# Typically you don't care so much for a third party library's examples to be
# run from your own project's code.
set(ERT_LOGGER_BuildExamples OFF CACHE INTERNAL "")

add_subdirectory(ertlogger)
...
add_library(foo ...)
...
target_link_libraries(foo PRIVATE ertlogger::ertlogger)
```

##### FetchContent

Since CMake v3.11,
[FetchContent](https://cmake.org/cmake/help/v3.11/module/FetchContent.html) can be used to automatically download the repository as a dependency at configure type.

Example:
```cmake
include(FetchContent)

FetchContent_Declare(ertlogger
  GIT_REPOSITORY https://github.com/testillano/logger.git
  GIT_TAG v1.0.3)

FetchContent_GetProperties(ertlogger)
if(NOT ert_json_POPULATED)
  FetchContent_Populate(ertlogger)
  add_subdirectory(${ertlogger_SOURCE_DIR} ${ertlogger_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_link_libraries(foo PRIVATE ertlogger::ertlogger)
```

### Build

    cmake .
    make

### Execute example

    build/Release/bin/logme

This will show the correct usage:

    usage: logme <log level>
    Log levels allowed: Debug|Informational|Notice|Warning|Error|Critical|Alert|Emergency

### Install

    sudo make install

### Contributing

Please, execute `astyle` formatting before any pull request:

    docker pull frankwolf/astyle
    docker run -it --rm -v $PWD:/data frankwolf/astyle include/ert/tracing/Logger.hpp src/Logger.cpp examples/main.cpp

