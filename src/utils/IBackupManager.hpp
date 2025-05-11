#ifndef IBACKUPMANAGER_HPP
#define IBACKUPMANAGER_HPP

#include <QString>

/**
 * @brief Interface for backup management.
 */
class IBackupManager {
public:
    virtual ~IBackupManager() = default;

    virtual void setBackupDir(const QString& dir) = 0;
    virtual void backupFile(const QString& srcPath) = 0;
    virtual void restoreFile(const QString& dstPath) = 0;
};

#endif // IBACKUPMANAGER_HPP
