// Project
#include "Logger.hh"

// C
#include <libgen.h> // basename
#include <signal.h>

// Standard
#include <iostream>
#include <string>
#include <algorithm>

const char* progname;

void _exit(int rc)
{
    TRACE(tst::tracing::Logger::Warning, "Terminating with exit code %d", rc)
    tst::tracing::Logger::terminate();
    exit(rc);
}

void sighndl(int signal)
{
    TRACE(tst::tracing::Logger::Warning, "Signal received: %d", signal)
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

    tst::tracing::Logger::initialize(progname);

    // Capture TERM/INT signals for graceful exit:
    signal(SIGTERM, sighndl);
    signal(SIGINT, sighndl);

    // Command-line
    std::string value = (argc > 1) ? argv[1]:"";
    if (value.empty()) usage(EXIT_FAILURE);

    if (!tst::tracing::Logger::setLevel(value))
    {
        usage(EXIT_FAILURE);
    }

    std::cout << "Level configured = " << tst::tracing::Logger::getLevel() << '\n';

    LOGDEBUG(
        std::string msg = tst::tracing::Logger::asString("This is LOGDEBUG (level %d)", tst::tracing::Logger::Debug);
        tst::tracing::Logger::debug(msg, TST_FILE_LOCATION);
    );
    LOGINFORMATIONAL(
        std::string msg = tst::tracing::Logger::asString("This is LOGINFORMATIONAL (level %d)", tst::tracing::Logger::Informational);
        tst::tracing::Logger::informational(msg, TST_FILE_LOCATION);
    );
    LOGNOTICE(
        std::string msg = tst::tracing::Logger::asString("This is LOGNOTICE (level %d)", tst::tracing::Logger::Notice);
        tst::tracing::Logger::notice(msg, TST_FILE_LOCATION);
    );
    LOGWARNING(
        std::string msg = tst::tracing::Logger::asString("This is LOGWARNING (level %d)", tst::tracing::Logger::Warning);
        tst::tracing::Logger::warning(msg, TST_FILE_LOCATION);
    );
    TRACE(tst::tracing::Logger::Debug, "This is TRACE for level %d", tst::tracing::Logger::Debug);
    TRACE(tst::tracing::Logger::Informational, "This is TRACE for level %d", tst::tracing::Logger::Informational);
    TRACE(tst::tracing::Logger::Notice, "This is TRACE for level %d", tst::tracing::Logger::Notice);
    TRACE(tst::tracing::Logger::Warning, "This is TRACE for level %d", tst::tracing::Logger::Warning);
    TRACE(tst::tracing::Logger::Error, "This is TRACE for level %d", tst::tracing::Logger::Error);
    TRACE(tst::tracing::Logger::Critical, "This is TRACE for level %d", tst::tracing::Logger::Critical);
    TRACE(tst::tracing::Logger::Alert, "This is TRACE for level %d", tst::tracing::Logger::Alert);
    TRACE(tst::tracing::Logger::Emergency, "This is TRACE for level %d", tst::tracing::Logger::Emergency);

    _exit(EXIT_SUCCESS);
}
