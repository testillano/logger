// C
#include <libgen.h> // basename
#include <signal.h>

// Standard
#include <iostream>
#include <string>
#include <algorithm>

#include <ert/Logger.hpp>

const char* progname;

void _exit(int rc)
{
    TRACE(ert::Logger::Warning, "Terminating with exit code %d", rc)
    ert::Logger::terminate();
    exit(rc);
}

void sighndl(int signal)
{
    TRACE(ert::Logger::Warning, "Signal received: %d", signal)
    _exit(EXIT_FAILURE);
}

void usage(int rc)
{
    auto& ss = (rc == 0) ? std::cout : std::cerr;

    ss << "usage: " << progname << " <log level>" << '\n'
       << "Log levels allowed: Debug|Informational|Notice|Warning|Error|Critical|Alert|Emergency" << '\n';

    _exit(rc);
}

int main(int argc, char* argv[]) {
    progname = basename(argv[0]);

    ert::Logger::initialize(progname);

    // Capture TERM/INT signals for graceful exit:
    signal(SIGTERM, sighndl);
    signal(SIGINT, sighndl);

    // Command-line
    std::string value = (argc > 1) ? argv[1]:"";
    if (value.empty()) usage(EXIT_FAILURE);

    if (!ert::Logger::setLevel(value))
    {
        usage(EXIT_FAILURE);
    }

    std::cout << "Level configured = " << ert::Logger::getLevel() << '\n';

    LOGDEBUG(
        std::string msg = ert::Logger::asString("This is LOGDEBUG (level %d)", ert::Logger::Debug);
        ert::Logger::debug(msg, TST_FILE_LOCATION);
    );
    LOGINFORMATIONAL(
        std::string msg = ert::Logger::asString("This is LOGINFORMATIONAL (level %d)", ert::Logger::Informational);
        ert::Logger::informational(msg, TST_FILE_LOCATION);
    );
    LOGNOTICE(
        std::string msg = ert::Logger::asString("This is LOGNOTICE (level %d)", ert::Logger::Notice);
        ert::Logger::notice(msg, TST_FILE_LOCATION);
    );
    LOGWARNING(
        std::string msg = ert::Logger::asString("This is LOGWARNING (level %d)", ert::Logger::Warning);
        ert::Logger::warning(msg, TST_FILE_LOCATION);
    );
    TRACE(ert::Logger::Debug, "This is TRACE for level %d", ert::Logger::Debug);
    TRACE(ert::Logger::Informational, "This is TRACE for level %d", ert::Logger::Informational);
    TRACE(ert::Logger::Notice, "This is TRACE for level %d", ert::Logger::Notice);
    TRACE(ert::Logger::Warning, "This is TRACE for level %d", ert::Logger::Warning);
    TRACE(ert::Logger::Error, "This is TRACE for level %d", ert::Logger::Error);
    TRACE(ert::Logger::Critical, "This is TRACE for level %d", ert::Logger::Critical);
    TRACE(ert::Logger::Alert, "This is TRACE for level %d", ert::Logger::Alert);
    TRACE(ert::Logger::Emergency, "This is TRACE for level %d", ert::Logger::Emergency);

    _exit(EXIT_SUCCESS);
}
