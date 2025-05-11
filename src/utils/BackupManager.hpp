#ifndef BACKUPMANAGER_HPP
#define BACKUPMANAGER_HPP

#include <QObject>
#include <QString>

#include "ILogger.hpp"
#include "IBackupManager.hpp"

/**
 * @brief Implements backup operations with logging.
 */
class BackupManager : public QObject, public IBackupManager {
    Q_OBJECT

public:
    BackupManager(ILogger* logger, QObject* parent = nullptr);

    void setBackupDir(const QString& dir) override;
    void backupFile(const QString& srcPath) override;
    void restoreFile(const QString& dstPath) override;

private:
    QString m_backupDir;
    ILogger* m_logger;
};

#endif // BACKUPMANAGER_HPP
