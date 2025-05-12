//===-- MonitorX/src/utils/BackupManager.hpp - Backup Service Interface --*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file defines the BackupManager class, which provides backup and restoration
// operations for monitored files. Logging is performed via ILogger.
//
//===-------------------------------------------------------------------------------===//

#ifndef BACKUPMANAGER_HPP
#define BACKUPMANAGER_HPP

#include "IBackupManager.hpp"
#include "ILogger.hpp"

#include <QObject>
#include <QString>

class BackupManager : public QObject, public IBackupManager {
    Q_OBJECT

public:
    explicit BackupManager(ILogger *logger, QObject *parent = nullptr);

    void setBackupDir(const QString &dir) override;
    void backupFile(const QString &srcPath) override;
    void restoreFile(const QString &dstPath) override;

private:
    QString m_backupDir;
    ILogger *m_logger;
};

#endif // BACKUPMANAGER_HPP
