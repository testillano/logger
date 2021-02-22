# C++ syslog wrapper library

The `syslog.h` library, could waste time parsing strings which are not going
to be finally logged. Indeed, `stdarg.h` is used behind so this could make
slower programs when using `syslog` traces.

This library is a simple wrapper static class over `syslog.h` in order to
check the log level in advance and also provides useful macro definitions
to ease messages formatting:

Sometimes you need to protect a whole block of code which only shall be
processed when the appropriate log level is assigned:

>    LOGDEBUG, LOGINFORMATIONAL, LOGNOTICE and LOGWARNING are used for that.
>

Other times, you already have information available and just need to trace
it inline:

>    TRACE is used for that.
>

### Build

    cmake .
    make

### Execute example

    ./logme

This will show the correct usage:

    Usage: logme <log level>

### Contributing

Please, execute `astyle` formatting before any pull request:

    docker pull frankwolf/astyle
    docker run -it --rm -v $PWD:/data frankwolf/astyle src/Logger.hh src/Logger.cc examples/main.cc

