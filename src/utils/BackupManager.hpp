#ifndef BACKUPMANAGER_HPP
#define BACKUPMANAGER_HPP

#include <QObject>
#include <QString>

/**
 * @brief Manages a simple file‐based backup directory.
 *
 * You must call setBackupDir() before using backupFile()/restoreFile().
 */
class BackupManager : public QObject {
    Q_OBJECT

public:
    explicit BackupManager(QObject* parent = nullptr);

    /// Set (and create) the directory where backups will be stored.
    void setBackupDir(const QString& dir);

    /// Copy @a srcPath into the backup directory (overwriting any existing file).
    void backupFile(const QString& srcPath);

    /// Restore from the backup directory back to @a dstPath (overwriting dstPath).
    void restoreFile(const QString& dstPath);

private:
    QString m_backupDir;
};

#endif // BACKUPMANAGER_HPP
