/*
            _          _
           | |        | |
   ___ _ __| |_   __  | |
  / _ \ '__| __| |__| | |/ _ \ / _` |/ _` |/ _ \ '__|  Syslog wrapper library C++
 |  __/ |  | |_       | | (_) | (_| | (_| |  __/ |     Version 1.0.0
  \___|_|   \__|      |_|\___/ \__, |\__, |\___|_|     https://github.com/testillano/logger
                                __/ | __/ |
                               |___/ |___/

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2004-2021 Eduardo Ramos <http://www.teslayout.com>.

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

#ifndef ERT_TRACING_LOGGER_H_
#define ERT_TRACING_LOGGER_H_

#include <mutex>
#include <string>

#include <syslog.h>

// Logger macros
#define ERT_FILE_LOCATION (const char *)__FILE__,(const int)__LINE__,(const char*)__func__

// Block-level-protected macros:
//
// Example:
//
// LOGDEBUG(
//   std::string trace;
//   ...
//   ...
//   ert::tracing::Logger::debug(msg, ERT_FILE_LOCATION);
// );
//
// LOG_EMERG =   0      system is unusable                   Emergency (emerg)
// LOG_ALERT =   1      action must be taken immediately     Alert (alert)
// LOG_CRIT =    2      critical conditions                  Critical (crit)
// LOG_ERR =     3      error conditions                     Error (err)
// LOG_WARNING = 4      warning conditions                   Warning (warning)
#define LOGWARNING(a) if (ert::tracing::Logger::isActive (ert::tracing::Logger::Warning)) {a;}
// LOG_NOTICE =  5      normal but significant condition     Notice (notice)
#define LOGNOTICE(a) if (ert::tracing::Logger::isActive (ert::tracing::Logger::Notice)) {a;}
// LOG_INFO =    6      informational                        Informational (info)
#define LOGINFORMATIONAL(a) if (ert::tracing::Logger::isActive (ert::tracing::Logger::Informational)) {a;}
// LOG_DEBUG =   7      debug-level messages                 Debug (debug)
#define LOGDEBUG(a) if (ert::tracing::Logger::isActive (ert::tracing::Logger::Debug)) {a;}

namespace ert {
namespace tracing {

/**
   Facility to generate application logs
*/
class Logger {
public:
    /**
       Trace level (Unix Posix Levels)
       See more at: man syslog.conf

       @see Logger
    */
    enum Level {
        Emergency = LOG_EMERG, Alert = LOG_ALERT, Critical = LOG_CRIT, Error = LOG_ERR,
        Warning = LOG_WARNING, Notice = LOG_NOTICE, Informational = LOG_INFO, Debug = LOG_DEBUG,
    };

    /**
       Initializes syslog system

       @param Program name (undefined by default)
       @param Syslog options (LOG_CONS by default)
       @param Syslog facility (LOG_LOCAL1 by default)
    */
    static void initialize(const char *programName = nullptr, int options = -1, int facility = -1) {
        openlog(programName, (options != -1) ? options:(LOG_CONS/* | LOG_PERROR*/), (facility != -1) ? facility:LOG_LOCAL1);
        initialized_ = true;
    }

    /**
       Terminates syslog system
    */
    static void terminate() {
        if(initialized_) closelog();
    }

    /**
       @return Current application trace level
    */
    static Level getLevel() {
        return level_;
    }

    /**
       Sets application trace level, warning by default.
       Only logs under this level will be processed.

       @param level Level desired.
    */
    static void setLevel(const Level level) {
        std::lock_guard<std::mutex> guard(mutex_);
        level_ = (level <= Error) ? Error : level;
        setlogmask(LOG_UPTO(level_)); // just in case syslog is used directly
    }

    /**
       Sets application trace level, warning by default.
       Only logs under this level will be processed.

       @param level Level description: Emergency, Alert, Critical, Error, Warning, Notice, Informational, Debug

       @return Boolean about successful operation
    */
    static bool setLevel(const std::string &level);

    /**
       Checks if application trace level is over provided one.
       For example, an info-level is active when application level is over (or equals) it: info, debug

       @param level Level to check

       @return @em true For levels with priority over application configured one, false in other case.
    */
    static bool isActive(const Level level) {
        return (level <= Error) ? true : (level <= level_);
    }
    static bool isActive(int level) {
        return isActive((Level)level);
    }

    /**
       Build a string with format and generic arguments
    */
    static std::string asString(const char* format, ...);

    /**
       Logs the text provided with a specific level when
       this level is active.

       @param level Trace level to register
       @param text Trace text
       @param fromFile File for trace
       @param fromLine Line for trace
       @param fromFunc Function for trace
    */
    static void log(const Level level, const char* text, const char* fromFile, const int fromLine, const char* fromFunc);
    static void log(const Level level, const std::string& text, const char* fromFile, const int fromLine, const char* fromFunc) {
        log(level, text.c_str(), fromFile, fromLine, fromFunc);
    }

    // Specific logger shortcuts:
    /**
       Logger shortcut for debug enabled level

       @param text Trace text
       @param fromFile File for trace
       @param fromLine Line for trace
       @param fromFunc Function for trace
    */
    static void debug(const std::string& text, const char* fromFile, const int fromLine, const char* fromFunc)
    {
        log(Logger::Debug, text, fromFile, fromLine, fromFunc);
    }
    /**
       Logger shortcut for informational enabled level

       @param text Trace text
       @param fromFile File for trace
       @param fromLine Line for trace
       @param fromFunc Function for trace
    */
    static void informational(const std::string& text, const char* fromFile, const int fromLine, const char* fromFunc)
    {
        log(Logger::Informational, text, fromFile, fromLine, fromFunc);
    }
    /**
       Logger shortcut for notice enabled level

       @param text Trace text
       @param fromFile File for trace
       @param fromLine Line for trace
       @param fromFunc Function for trace
    */
    static void notice(const std::string& text, const char* fromFile, const int fromLine, const char* fromFunc)
    {
        log(Logger::Notice, text, fromFile, fromLine, fromFunc);
    }
    /**
       Logger shortcut for warning enabled level

       @param text Trace text
       @param fromFile File for trace
       @param fromLine Line for trace
       @param fromFunc Function for trace
    */
    static void warning(const std::string& text, const char* fromFile, const int fromLine, const char* fromFunc)
    {
        log(Logger::Warning, text, fromFile, fromLine, fromFunc);
    }
    /**
       Logger shortcut for error enabled level

       @param text Trace text
       @param fromFile File for trace
       @param fromLine Line for trace
       @param fromFunc Function for trace
    */
    static void error(const std::string& text, const char* fromFile, const int fromLine, const char* fromFunc)
    {
        log(Logger::Error, text, fromFile, fromLine, fromFunc);
    }
    /**
       Logger shortcut for critical enabled level

       @param text Trace text
       @param fromFile File for trace
       @param fromLine Line for trace
       @param fromFunc Function for trace
    */
    static void critical(const std::string& text, const char* fromFile, const int fromLine, const char* fromFunc)
    {
        log(Logger::Critical, text, fromFile, fromLine, fromFunc);
    }
    /**
       Logger shortcut for alert enabled level

       @param text Trace text
       @param fromFile File for trace
       @param fromLine Line for trace
       @param fromFunc Function for trace
    */
    static void alert(const std::string& text, const char* fromFile, const int fromLine, const char* fromFunc)
    {
        log(Logger::Alert, text, fromFile, fromLine, fromFunc);
    }
    /**
       Logger shortcut for emergency enabled level

       @param text Trace text
       @param fromFile File for trace
       @param fromLine Line for trace
       @param fromFunc Function for trace
    */
    static void emergency(const std::string& text, const char* fromFile, const int fromLine, const char* fromFunc)
    {
        log(Logger::Emergency, text, fromFile, fromLine, fromFunc);
    }

    /**
       Translates internal level value into string representation.

       @return String which identifies the level value received as parameter

       @warning We are not using syslog standard but our propietary slogans to keep compatibility
    */
    static const char* levelAsString(const Level level);

    /**
       Translates string representation for level into internal value.
       Log levels allowed: Debug|Informational|Notice|Warning|Error|Critical|Alert|Emergency

       @return Level which identifies the level string received as parameter. Return -1 when level is unsupported.

       @warning We are not using syslog standard but our propietary slogans to keep compatibility
    */
    static Level stringAsLevel(const std::string& level);

private:
    static std::mutex mutex_;
    static Level level_;
    static bool initialized_;

    Logger() {};
    ~Logger() {};
};

}
}

#endif

