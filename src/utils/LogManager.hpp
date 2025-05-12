//===-- MonitorX/src/utils/LogManager.hpp - File-based Event Logger -*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file defines the LogManager class, which implements the ILogger
// interface to write timestamped log entries into a persistent file.
// It uses Qt's file and time utilities to ensure reliability.
//
//===--------------------------------------------------------------------------===//

#ifndef LOGMANAGER_HPP
#define LOGMANAGER_HPP

#include "ILogger.hpp"

#include <QObject>

class LogManager : public QObject, public ILogger {
    Q_OBJECT

public:
    explicit LogManager(const QString &logFilePath, QObject *parent = nullptr);

    void logEvent(const QString &event, const QString &filePath) override;

private:
    QString m_logFilePath;
};

#endif // LOGMANAGER_HPP
