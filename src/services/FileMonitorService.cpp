#include "FileMonitorService.hpp"
#include "ConfigManager.hpp"
#include "HashManager.hpp"
#include "BackupManager.hpp"
#include "LogManager.hpp"

#include <QDir>
#include <QFileInfo>
#include <QFile>

FileMonitorService::FileMonitorService(ConfigManager* cfg, QObject* parent)
    : QObject(parent)
    , m_cfg(cfg)
    , m_backup(new BackupManager(this))
    , m_log(new LogManager(cfg->logFile(), this))
{
    QDir watchDir(m_cfg->watchDirectory());
    QString backupPath = watchDir.filePath(m_cfg->backupDirectory());
    m_backup->setBackupDir(backupPath);

    connect(&m_watcher, &QFileSystemWatcher::directoryChanged,
            this, &FileMonitorService::onDirectoryChanged);
    connect(&m_watcher, &QFileSystemWatcher::fileChanged,
            this, &FileMonitorService::onFileChanged);

    connect(m_cfg, &ConfigManager::configChanged, this, [this](){
        if (m_running) {
            stop();
            start();
        }
    });
}


FileMonitorService::~FileMonitorService()
{
    stop();
}

void FileMonitorService::start()
{
    if (m_running) return;

    m_knownGroups.clear();
    m_groupHashes.clear();
    m_watcher.removePaths(m_watcher.directories());
    m_watcher.removePaths(m_watcher.files());

    initialScan();
    m_watcher.addPath(m_cfg->watchDirectory());
    m_running = true;
}

void FileMonitorService::stop()
{
    if (!m_running) return;
    m_watcher.removePaths(m_watcher.directories());
    m_watcher.removePaths(m_watcher.files());
    m_knownGroups.clear();
    m_groupHashes.clear();
    m_running = false;
}

void FileMonitorService::initialScan()
{
    QDir dir(m_cfg->watchDirectory());
    const QStringList exts = m_cfg->extensions();
    const QFileInfoList fis = dir.entryInfoList(exts, QDir::Files | QDir::NoSymLinks);

    QHash<QString, QStringList> groups;
    for (const QFileInfo& fi : fis) {
        groups[fi.completeBaseName()].append(fi.absoluteFilePath());
    }

    for (auto it = groups.constBegin(); it != groups.constEnd(); ++it) {
        const QString   groupName = it.key();
        const QStringList paths   = it.value();
        m_knownGroups.insert(groupName);

        QHash<QString,QByteArray> hashMap;
        for (const QString& path : paths) {
            QByteArray h = HashManager::fileHash(path, m_cfg->hashAlg());
            hashMap[path] = h;
            m_backup->backupFile(path);
            m_watcher.addPath(path);
        }
        m_groupHashes[groupName] = hashMap;

        m_log->logEvent("Added", groupName);
        emit fileAdded(groupName);
    }
}

void FileMonitorService::onDirectoryChanged(const QString&)
{
    QDir dir(m_cfg->watchDirectory());
    const QStringList exts = m_cfg->extensions();
    const QFileInfoList fis = dir.entryInfoList(exts, QDir::Files | QDir::NoSymLinks);

    QHash<QString, QStringList> groups;
    for (const QFileInfo& fi : fis) {
        groups[fi.completeBaseName()].append(fi.absoluteFilePath());
    }

    for (auto it = groups.constBegin(); it != groups.constEnd(); ++it) {
        const QString groupName = it.key();
        if (!m_knownGroups.contains(groupName)) {
            m_knownGroups.insert(groupName);
            const QStringList paths = it.value();

            QHash<QString,QByteArray> hashMap;
            for (const QString& path : paths) {
                QByteArray h = HashManager::fileHash(path, m_cfg->hashAlg());
                hashMap[path] = h;
                m_backup->backupFile(path);
                m_watcher.addPath(path);
            }
            m_groupHashes[groupName] = hashMap;

            m_log->logEvent("Added", groupName);
            emit fileAdded(groupName);
        }
    }
}

void FileMonitorService::onFileChanged(const QString& path)
{
    QFileInfo fi(path);
    const QString groupName = fi.completeBaseName();
    if (!m_knownGroups.contains(groupName))
        return;

    auto& hashMap   = m_groupHashes[groupName];
    const QStringList paths = hashMap.keys();

    bool anyExist = false;
    for (const QString& p : paths) {
        if (QFile::exists(p)) { anyExist = true; break; }
    }

    if (!anyExist) {
        m_log->logEvent("Deleted", groupName);
        for (const QString& p : paths)
            m_backup->restoreFile(p);
        m_log->logEvent("Restored", groupName);
        emit fileRestored(groupName);

        for (const QString& p : paths)
            m_watcher.addPath(p);

    } else {
        bool modified = false;
        for (const QString& p : paths) {
            if (!QFile::exists(p)) continue;
            QByteArray newH = HashManager::fileHash(p, m_cfg->hashAlg());
            if (newH != hashMap[p]) {
                hashMap[p] = newH;
                modified = true;
            }
        }
        if (modified) {
            for (const QString& p : paths)
                m_backup->backupFile(p);
            m_log->logEvent("Modified", groupName);
            emit fileUpdated(groupName);
        }
    }
}
