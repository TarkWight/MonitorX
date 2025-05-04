#include "BackupManager.hpp"
#include <QFile>
#include <QDir>
#include <QFileInfo>

BackupManager::BackupManager(QObject* parent)
    : QObject(parent)
{}

void BackupManager::setBackupDir(const QString& dir)
{
    m_backupDir = dir;
    QDir().mkpath(m_backupDir);
}

void BackupManager::backupFile(const QString& srcPath)
{
    if (m_backupDir.isEmpty())
        return;

    QFileInfo fi(srcPath);
    QString dest = QDir(m_backupDir).filePath(fi.fileName());

    //  existing backup
    QFile::remove(dest);
    QFile::copy(srcPath, dest);
}

void BackupManager::restoreFile(const QString& dstPath)
{
    if (m_backupDir.isEmpty())
        return;

    QFileInfo fi(dstPath);
    QString src = QDir(m_backupDir).filePath(fi.fileName());
    if (!QFile::exists(src))
        return;

    QFile::remove(dstPath);
    QFile::copy(src, dstPath);
}
