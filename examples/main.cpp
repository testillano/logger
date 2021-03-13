/*
 _________________________________________________________
|             _          _                               |
|            | |        | |                              |
|    ___ _ __| |_   __  | | ___   __ _  __ _  ___ _ __   |
|   / _ \ '__| __| |__| | |/ _ \ / _` |/ _` |/ _ \ '__|  |  Syslog wrapper library C++
|  |  __/ |  | |_       | | (_) | (_| | (_| |  __/ |     |  Version 1.0.z
|   \___|_|   \__|      |_|\___/ \__, |\__, |\___|_|     |  https://github.com/testillano/logger
|                                 __/ | __/ |            |
|                                |___/ |___/             |
|________________________________________________________|

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2021 Eduardo Ramos

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// C
#include <libgen.h> // basename
#include <signal.h>

// Standard
#include <iostream>
#include <string>
#include <algorithm>

#include <ert/tracing/Logger.hpp>

const char* progname;

void _exit(int rc)
{
    LOGWARNING(ert::tracing::Logger::warning(ert::tracing::Logger::asString("Terminating with exit code %d", rc), ERT_FILE_LOCATION));
    ert::tracing::Logger::terminate();
    exit(rc);
}

void sighndl(int signal)
{
    LOGWARNING(ert::tracing::Logger::warning(ert::tracing::Logger::asString("Signal received: %d", signal), ERT_FILE_LOCATION));
    _exit(EXIT_FAILURE);
}

void usage(int rc)
{
    auto& ss = (rc == 0) ? std::cout : std::cerr;

    ss << '\n' << "Usage: " << progname << " <log level> [--verbose (to print traces on console)]" << '\n'
       << '\n' << "Log levels allowed: Debug|Informational|Notice|Warning|Error|Critical|Alert|Emergency" << '\n';

    _exit(rc);
}

int main(int argc, char* argv[]) {
    progname = basename(argv[0]);

    ert::tracing::Logger::initialize(progname);

    // Capture TERM/INT signals for graceful exit:
    signal(SIGTERM, sighndl);
    signal(SIGINT, sighndl);

    // Command-line
    std::string value = (argc > 1) ? argv[1]:"";
    if (value.empty()) usage(EXIT_FAILURE);

    if (!ert::tracing::Logger::setLevel(value))
    {
        usage(EXIT_FAILURE);
    }
    std::string verbose = (argc > 2) ? argv[2]:"";
    if (verbose == "--verbose") ert::tracing::Logger::verbose();


    std::cout << "Level configured = " << ert::tracing::Logger::getLevel() << '\n';

    LOGDEBUG(
        std::string msg = ert::tracing::Logger::asString("This is DEBUG (level %d)", ert::tracing::Logger::Debug);
        ert::tracing::Logger::debug(msg, ERT_FILE_LOCATION);
    );
    LOGINFORMATIONAL(
        std::string msg = ert::tracing::Logger::asString("This is INFORMATIONAL (level %d)", ert::tracing::Logger::Informational);
        ert::tracing::Logger::informational(msg, ERT_FILE_LOCATION);
    );
    LOGNOTICE(
        std::string msg = ert::tracing::Logger::asString("This is NOTICE (level %d)", ert::tracing::Logger::Notice);
        ert::tracing::Logger::notice(msg, ERT_FILE_LOCATION);
    );
    LOGWARNING(
        std::string msg = ert::tracing::Logger::asString("This is WARNING (level %d)", ert::tracing::Logger::Warning);
        ert::tracing::Logger::warning(msg, ERT_FILE_LOCATION);
    );

    std::string msg;

    msg = ert::tracing::Logger::asString("This is ERROR (level %d)", ert::tracing::Logger::Error);
    ert::tracing::Logger::error(msg, ERT_FILE_LOCATION);

    msg = ert::tracing::Logger::asString("This is CRITICAL (level %d)", ert::tracing::Logger::Critical);
    ert::tracing::Logger::critical(msg, ERT_FILE_LOCATION);

    msg = ert::tracing::Logger::asString("This is ALERT (level %d)", ert::tracing::Logger::Alert);
    ert::tracing::Logger::alert(msg, ERT_FILE_LOCATION);

    // Commented to avoid journal broadcasting:
    //msg = ert::tracing::Logger::asString("This is EMERGENCY (level %d)", ert::tracing::Logger::Emergency);
    //ert::tracing::Logger::emergency(msg, ERT_FILE_LOCATION);

    _exit(EXIT_SUCCESS);
}
