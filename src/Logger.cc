/*
  _       _          _
 | |     | |        | |
 | |_ ___| |_   __  | | ___   __ _  __ _  ___ _ __
 | __/ __| __| |__| | |/ _ \ / _` |/ _` |/ _ \ '__|   Syslog wrapper library C++
 | |_\__ \ |_       | | (_) | (_| | (_| |  __/ |      Version 1.0.0
  \__|___/\__|      |_|\___/ \__, |\__, |\___|_|      https://github.com/testillano/logger
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

#include "Logger.hh"

#include <stdio.h>
#include <stdarg.h>

namespace ert {
namespace tracing {

std::mutex Logger::mutex_;
Logger::Level Logger::level_ = Logger::Warning;
bool Logger::initialized_ = false;

std::string Logger::asString(const char* format, ...)
{
    va_list ap;
    char aux [1024];
    va_start(ap, format);
    vsnprintf(aux, sizeof(aux), format, ap);
    va_end(ap);
    return std::string(aux);
}

bool Logger::setLevel(const std::string &level) {
    if (level == "Debug") setLevel(Debug);
    else if (level == "Informational") setLevel(Informational);
    else if (level == "Notice") setLevel(Notice);
    else if (level == "Warning") setLevel(Warning);
    else if (level == "Error") setLevel(Error);
    else if (level == "Critical") setLevel(Critical);
    else if (level == "Alert") setLevel(Alert);
    else if (level == "Emergency") setLevel(Emergency);
    else return false;

    return true;
}

void Logger::write(const Level level, const char* text, const char* fromFile, const int fromLine, const char* fromFunc)
{
    if(isActive(level)) {
        const char *s_level = levelAsString(level);
        syslog(level, "[%s]|%s:%d(%s)|%s", (s_level ? s_level:"<level not supported>"), fromFile, fromLine, fromFunc, text);
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

}
}


