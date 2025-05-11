#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <QString>

/**
 * @brief Interface for logging events.
 */
class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void logEvent(const QString& event, const QString& filePath) = 0;
};

#endif // ILOGGER_HPP
