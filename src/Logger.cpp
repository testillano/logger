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

#include <stdio.h>
#include <stdarg.h>

#include <iostream>

#include <ert/tracing/Logger.hpp>


namespace ert {
namespace tracing {

std::mutex Logger::mutex_;
Logger::Level Logger::level_ = Logger::Warning;
bool Logger::initialized_ = false;
bool Logger::verbose_ = false;

std::string Logger::asString(const char* format, ...)
{
    va_list ap;
    char aux [4096];
    va_start(ap, format);
    vsnprintf(aux, sizeof(aux), format, ap);
    va_end(ap);
    return std::string(aux);
}

bool Logger::setLevel(const std::string &level) {

    Level value = stringAsLevel(level);

    if (value == -1) return false;

    setLevel(value);

    return true;
}

void Logger::log(const Level level, const char* text, const char* fromFile, const int fromLine, const char* fromFunc)
{
    if(isActive(level)) {
        const char *s_level = levelAsString(level);
        syslog(level, "[%s]|%s:%d(%s)|%s", (s_level ? s_level:"<level not supported>"), fromFile, fromLine, fromFunc, text);

        if(verbose_) {
          std::string trace = asString("[%s]|%s:%d(%s)|%s", (s_level ? s_level:"<level not supported>"), fromFile, fromLine, fromFunc, text);
          (level <= Error ? std::cerr:std::cout) << trace << '\n';
        }
    }
}

const char* Logger::levelAsString(const Level level)
{
    const char* result = NULL;

    // SYSLOG:
    // Severities: Emergency, Alert, Critical, Error, Warning, Notice, Informational, Debug
    // Keywords:   emerg      alert  crit      err    warning  notice  info           debug

    static const char* levels [] = {
        "Emergency", "Alert", "Critical", "Error", "Warning", "Notice", "Informational", "Debug"
    };

    if(level >= Emergency && level <= Debug)
        result = levels [level];

    return result;
}

Logger::Level Logger::stringAsLevel(const std::string& level)
{
    if (level == "Debug")  return Debug;
    else if (level == "Informational") return Informational;
    else if (level == "Notice") return Notice;
    else if (level == "Warning") return Warning;
    else if (level == "Error") return Error;
    else if (level == "Critical") return Critical;
    else if (level == "Alert") return Alert;
    else if (level == "Emergency") return Emergency;

    return (Logger::Level)-1;
}

}
}


