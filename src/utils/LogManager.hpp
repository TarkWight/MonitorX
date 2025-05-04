#ifndef LOGMANAGER_HPP
#define LOGMANAGER_HPP

#include <QObject>
#include <QString>

/**
 * @brief Appends timestamped log entries into a text file.
 */
class LogManager : public QObject {
    Q_OBJECT

public:
    /**
     * @param logFilePath  Path to a .txt file where logs will be written.
     *                     The directory will be created if necessary.
     */
    explicit LogManager(const QString& logFilePath, QObject* parent = nullptr);

    /**
     * @brief Append a new log entry in the format:
     *        [YYYY-MM-DD hh:mm:ss] <event>: <filePath>
     */
    void logEvent(const QString& event, const QString& filePath);

private:
    QString m_logFilePath;
};

#endif // LOGMANAGER_HPP
