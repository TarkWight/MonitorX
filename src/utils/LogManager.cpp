//===-- MonitorX/src/utils/LogManager.cpp - File-based Event Logger Implementation -*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// Implements the LogManager class, which writes timestamped log entries into a persistent file
// for audit and debugging purposes.
//
//===-----------------------------------------------------------------------------------------===//

#include "LogManager.hpp"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

LogManager::LogManager(const QString &logFilePath, QObject *parent)
    : QObject(parent), m_logFilePath(logFilePath) {
    QFileInfo fi(m_logFilePath);
    QDir().mkpath(fi.absolutePath());

    QFile f(m_logFilePath);
    if (!f.exists()) {
        f.open(QIODevice::WriteOnly);
        f.close();
    }
}

void LogManager::logEvent(const QString &event, const QString &filePath) {
    QFile f(m_logFilePath);
    if (!f.open(QIODevice::Append | QIODevice::Text)) {
        return;
    }

    QTextStream out(&f);
    out << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        << "] " << event << ": " << filePath << "\n";
    f.close();
}
