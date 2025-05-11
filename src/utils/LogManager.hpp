#ifndef LOGMANAGER_HPP
#define LOGMANAGER_HPP

#include "ILogger.hpp"

#include <QObject>

/**
 * @brief Appends timestamped log entries into a text file.
 */
class LogManager : public QObject, public ILogger {
    Q_OBJECT

public:
    /**
     * @param logFilePath  Path to a .txt file where logs will be written.
     *                     The directory will be created if necessary.
     */
    explicit LogManager(const QString& logFilePath, QObject* parent = nullptr);

    // ILogger interface
    void logEvent(const QString& event, const QString& filePath) override;

private:
    QString m_logFilePath;
};

#endif // LOGMANAGER_HPP
