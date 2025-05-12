//===-- MonitorX/src/utils/IBackupManager.hpp - Backup Interface Definition --*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file defines the IBackupManager interface used to abstract the backup logic in the
// system. It provides methods for setting the backup directory, performing backups,
// and restoring files.
//
//===-----------------------------------------------------------------------------------===//

#ifndef IBACKUPMANAGER_HPP
#define IBACKUPMANAGER_HPP

#include <QString>

class IBackupManager {
public:
    virtual ~IBackupManager() = default;

    virtual void setBackupDir(const QString &dir) = 0;
    virtual void backupFile(const QString &srcPath) = 0;
    virtual void restoreFile(const QString &dstPath) = 0;
};

#endif // IBACKUPMANAGER_HPP
