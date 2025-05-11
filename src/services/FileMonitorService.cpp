#include "FileMonitorService.hpp"

#include <QDir>
#include <QFileInfo>
#include <QFile>

FileMonitorService::FileMonitorService(IConfigManager* cfg,
                                       IBackupManager* backup,
                                       IHashManager* hasher,
                                       ILogger* logger,
                                       QObject* parent)
    : IFileMonitorService(parent)
    , m_cfg(cfg)
    , m_backup(backup)
    , m_hasher(hasher)
    , m_log(logger)
{
    QDir watchDir(m_cfg->watchDirectory());
    QString backupPath = watchDir.filePath(m_cfg->backupDirectory());
    m_backup->setBackupDir(backupPath);
    m_log->logEvent("Monitor initialized for", m_cfg->watchDirectory());

    connect(&m_watcher, &QFileSystemWatcher::directoryChanged,
            this, &FileMonitorService::onDirectoryChanged);
    connect(&m_watcher, &QFileSystemWatcher::fileChanged,
            this, &FileMonitorService::onFileChanged);

    connect(m_cfg, &IConfigManager::configChanged, this, &FileMonitorService::stop);
    connect(m_cfg, &QObject::destroyed, this, &FileMonitorService::stop);
}

// D:\Develop\Source\QtProjects\MonitorX\src\services\FileMonitorService.cpp:34:5: Call to virtual method 'FileMonitorService::stop' during destruction bypasses virtual dispatch [clang-analyzer-optin.cplusplus.VirtualCall]
FileMonitorService::~FileMonitorService()
{
    m_log->logEvent("Monitor destroyed", m_cfg->watchDirectory());
}


void FileMonitorService::start()
{
    if (m_running)
        return;

    m_log->logEvent("Starting monitor", m_cfg->watchDirectory());
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
    if (!m_running) {
        return;
    }

    m_log->logEvent("Stopping monitor", m_cfg->watchDirectory());
    m_watcher.removePaths(m_watcher.directories());
    m_watcher.removePaths(m_watcher.files());
    m_knownGroups.clear();
    m_groupHashes.clear();
    m_running = false;
}

bool FileMonitorService::isRunning() const
{
    return m_running;
}

void FileMonitorService::initialScan() {
    // TODO: - remove
    m_log->logEvent("Scanning directory", m_cfg->watchDirectory());

    QDir dir(m_cfg->watchDirectory());
    const QStringList exts = m_cfg->extensions();

    // TODO: - remove
    for (const auto& ext : exts)
        m_log->logEvent("Using filter", ext);


    const QFileInfoList fis = dir.entryInfoList(exts, QDir::Files | QDir::NoSymLinks);

    // TODO: - remove
    m_log->logEvent("Files found", QString::number(fis.size()));

    QHash<QString, QStringList> groups;
    for (const QFileInfo& fi : fis)
        groups[fi.completeBaseName()].append(fi.absoluteFilePath());

    for (auto it = groups.constBegin(); it != groups.constEnd(); ++it) {
        const QString groupName = it.key();
        const QStringList paths = it.value();
        m_knownGroups.insert(groupName);

        QHash<QString, QByteArray> hashMap;
        for (const QString& path : paths) {
            QByteArray h = m_hasher->fileHash(path, m_cfg->hashAlg());
            hashMap[path] = h;
            m_backup->backupFile(path);
            m_watcher.addPath(path);
        }
        m_groupHashes[groupName] = hashMap;

        m_log->logEvent("Added group", groupName);
        emit fileAdded(groupName);
    }
}

void FileMonitorService::onDirectoryChanged(const QString&)
{
    QDir dir(m_cfg->watchDirectory());
    const QStringList exts = m_cfg->extensions();
    const QFileInfoList fis = dir.entryInfoList(exts, QDir::Files | QDir::NoSymLinks);

    QHash<QString, QStringList> groups;
    for (const QFileInfo& fi : fis)
        groups[fi.completeBaseName()].append(fi.absoluteFilePath());

    for (auto it = groups.constBegin(); it != groups.constEnd(); ++it) {
        const QString groupName = it.key();
        if (!m_knownGroups.contains(groupName)) {
            m_knownGroups.insert(groupName);
            const QStringList paths = it.value();

            QHash<QString, QByteArray> hashMap;
            for (const QString& path : paths) {
                QByteArray h = m_hasher->fileHash(path, m_cfg->hashAlg());
                hashMap[path] = h;
                m_backup->backupFile(path);
                m_watcher.addPath(path);
            }
            m_groupHashes[groupName] = hashMap;

            m_log->logEvent("New group added", groupName);
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

    auto& hashMap = m_groupHashes[groupName];
    const QStringList paths = hashMap.keys();

    bool anyExist = false;
    for (const QString& p : paths) {
        if (QFile::exists(p)) {
            anyExist = true;
            break;
        }
    }

    if (!anyExist) {
        m_log->logEvent("Group deleted", groupName);
        for (const QString& p : paths)
            m_backup->restoreFile(p);
        m_log->logEvent("Group restored", groupName);
        emit fileRestored(groupName);

        for (const QString& p : paths)
            m_watcher.addPath(p);
    } else {
        bool modified = false;
        for (const QString& p : paths) {
            if (!QFile::exists(p))
                continue;
            QByteArray newH = m_hasher->fileHash(p, m_cfg->hashAlg());
            if (newH != hashMap[p]) {
                hashMap[p] = newH;
                modified = true;
            }
        }

        if (modified) {
            for (const QString& p : paths)
                m_backup->backupFile(p);
            m_log->logEvent("Group modified", groupName);
            emit fileUpdated(groupName);
        }
    }
}
