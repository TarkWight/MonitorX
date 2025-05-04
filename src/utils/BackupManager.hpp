#ifndef BACKUPMANAGER_HPP
#define BACKUPMANAGER_HPP

#include <QObject>
#include <QString>
#include <QPointer>

class LogManager;

/**
 * @brief Manages a simple file‐based backup directory.
 *
 * Handles creation of backups and restoring files from backups.
 * Logs all operations and errors via LogManager (if attached).
 *
 * Usage:
 *  - Set backup directory using setBackupDir().
 *  - (Optional) Attach logger using setLogger().
 *  - Use backupFile() and restoreFile() for backup operations.
 */
class BackupManager : public QObject {
    Q_OBJECT

public:
    explicit BackupManager(QObject* parent = nullptr);

    /**
     * @brief Sets (and creates) the directory where backups will be stored.
     *
     * If the directory does not exist, it will be created automatically.
     */
    void setBackupDir(const QString& dir);

    /**
     * @brief Sets a logger to record backup and restore events.
     *
     * Logger is optional. If not set, events will not be logged.
     */
    void setLogger(LogManager* logger);

    /**
     * @brief Copies srcPath into the backup directory.
     *
     * Overwrites any existing file with the same name.
     * Logs success or failure.
     */
    void backupFile(const QString& srcPath);

    /**
     * @brief Restores file from backup directory to dstPath.
     *
     * Overwrites any existing file at dstPath.
     * Logs success or failure.
     */
    void restoreFile(const QString& dstPath);

private:
    QString m_backupDir;
    QPointer<LogManager> m_logger;
};

#endif // BACKUPMANAGER_HPP
