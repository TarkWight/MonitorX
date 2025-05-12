//===-- MonitorX/src/utils/ILogger.hpp - Logger Interface --*- C++ ----*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file defines the ILogger interface, which is responsible for
// handling application log events. Implementations can write to files,
// consoles, or other logging backends.
//
//===--------------------------------------------------------------------===//

#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <QString>

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void logEvent(const QString &event, const QString &filePath) = 0;
};

#endif // ILOGGER_HPP
