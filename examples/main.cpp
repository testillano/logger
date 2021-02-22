// C
#include <libgen.h> // basename
#include <signal.h>

// Standard
#include <iostream>
#include <string>
#include <algorithm>

#include <tracing/Logger.hpp>

const char* progname;

void _exit(int rc)
{
    TRACE(ert::tracing::Logger::Warning, "Terminating with exit code %d", rc)
    ert::tracing::Logger::terminate();
    exit(rc);
}

void sighndl(int signal)
{
    TRACE(ert::tracing::Logger::Warning, "Signal received: %d", signal)
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

    std::cout << "Level configured = " << ert::tracing::Logger::getLevel() << '\n';

    LOGDEBUG(
        std::string msg = ert::tracing::Logger::asString("This is LOGDEBUG (level %d)", ert::tracing::Logger::Debug);
        ert::tracing::Logger::debug(msg, ERT_FILE_LOCATION);
    );
    LOGINFORMATIONAL(
        std::string msg = ert::tracing::Logger::asString("This is LOGINFORMATIONAL (level %d)", ert::tracing::Logger::Informational);
        ert::tracing::Logger::informational(msg, ERT_FILE_LOCATION);
    );
    LOGNOTICE(
        std::string msg = ert::tracing::Logger::asString("This is LOGNOTICE (level %d)", ert::tracing::Logger::Notice);
        ert::tracing::Logger::notice(msg, ERT_FILE_LOCATION);
    );
    LOGWARNING(
        std::string msg = ert::tracing::Logger::asString("This is LOGWARNING (level %d)", ert::tracing::Logger::Warning);
        ert::tracing::Logger::warning(msg, ERT_FILE_LOCATION);
    );
    TRACE(ert::tracing::Logger::Debug, "This is TRACE for level %d", ert::tracing::Logger::Debug);
    TRACE(ert::tracing::Logger::Informational, "This is TRACE for level %d", ert::tracing::Logger::Informational);
    TRACE(ert::tracing::Logger::Notice, "This is TRACE for level %d", ert::tracing::Logger::Notice);
    TRACE(ert::tracing::Logger::Warning, "This is TRACE for level %d", ert::tracing::Logger::Warning);
    TRACE(ert::tracing::Logger::Error, "This is TRACE for level %d", ert::tracing::Logger::Error);
    TRACE(ert::tracing::Logger::Critical, "This is TRACE for level %d", ert::tracing::Logger::Critical);
    TRACE(ert::tracing::Logger::Alert, "This is TRACE for level %d", ert::tracing::Logger::Alert);
    TRACE(ert::tracing::Logger::Emergency, "This is TRACE for level %d", ert::tracing::Logger::Emergency);

    _exit(EXIT_SUCCESS);
}
