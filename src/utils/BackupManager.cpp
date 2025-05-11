#include "BackupManager.hpp"

#include <QFile>
#include <QDir>
#include <QFileInfo>

BackupManager::BackupManager(ILogger* logger, QObject* parent)
    : QObject(parent)
    , m_logger(logger)
{}

void BackupManager::setBackupDir(const QString& dir)
{
    m_backupDir = dir;
    QDir().mkpath(m_backupDir);
    if (m_logger) {
        m_logger->logEvent("Backup directory set", m_backupDir);
    }
}

void BackupManager::backupFile(const QString& srcPath)
{
    if (m_backupDir.isEmpty()) {
        if (m_logger) {
            m_logger->logEvent("Backup FAILED (backupDir empty)", srcPath);
        }
        return;
    }

    QFileInfo fi(srcPath);
    QString dest = QDir(m_backupDir).filePath(fi.fileName());

    QFile::remove(dest);
    if (QFile::copy(srcPath, dest)) {
        if (m_logger) {
            m_logger->logEvent("Backup created", dest);
        }
    } else {
        if (m_logger) {
            m_logger->logEvent("Backup FAILED", dest);
        }
    }
}

void BackupManager::restoreFile(const QString& dstPath)
{
    if (m_backupDir.isEmpty()) {
        if (m_logger) {
            m_logger->logEvent("Restore FAILED (backupDir empty)", dstPath);
        }

        return;
    }

    QFileInfo fi(dstPath);
    QString src = QDir(m_backupDir).filePath(fi.fileName());

    if (!QFile::exists(src)) {
        if (m_logger) {
            m_logger->logEvent("Backup not found (restore failed)", dstPath);
        }

        return;
    }

    QFile::remove(dstPath);
    if (QFile::copy(src, dstPath)) {
        if (m_logger) {
            m_logger->logEvent("Restored from backup", dstPath);
        }
    } else {
        if (m_logger) {
            m_logger->logEvent("Restore FAILED", dstPath);
        }
    }
}
