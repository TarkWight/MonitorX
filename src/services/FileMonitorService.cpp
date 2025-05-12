//===-- MonitorX/src/services/FileMonitorService.cpp - Save Group Monitoring --*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file implements FileMonitorService, a Qt-based service responsible for
// monitoring changes in grouped save files inside a configured directory.
// It reacts to additions, deletions, and modifications by emitting signals
// and managing backup/restoration.
//
//===------------------------------------------------------------------------------------===//

#include "FileMonitorService.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QElapsedTimer>

FileMonitorService::FileMonitorService(IConfigManager *cfg,
                                       IBackupManager *backup,
                                       IHashManager *hasher,
                                       ILogger *logger,
                                       QObject *parent)
    : IFileMonitorService(parent),
    m_cfg(cfg),
    m_backup(backup),
    m_hasher(hasher),
    m_log(logger) {
    QDir watchDir(m_cfg->watchDirectory());
    const QString backupPath = watchDir.filePath(m_cfg->backupDirectory());
    m_backup->setBackupDir(backupPath);
    m_log->logEvent("Monitor initialized for", m_cfg->watchDirectory());

    connect(&m_watcher, &QFileSystemWatcher::directoryChanged,
            this, &FileMonitorService::onDirectoryChanged);
    connect(&m_watcher, &QFileSystemWatcher::fileChanged,
            this, &FileMonitorService::onFileChanged);

    connect(m_cfg, &IConfigManager::configChanged, this, &FileMonitorService::stop);
    connect(m_cfg, &QObject::destroyed, this, &FileMonitorService::stop);
}

FileMonitorService::~FileMonitorService() {
    m_log->logEvent("Monitor destroyed", m_cfg->watchDirectory());
}

void FileMonitorService::start() {
    if (m_running) {
        return;
    }

    m_log->logEvent("Starting monitor", m_cfg->watchDirectory());

    m_knownGroups.clear();
    m_groupHashes.clear();
    m_watcher.removePaths(m_watcher.directories());
    m_watcher.removePaths(m_watcher.files());

    initialScan();
    m_watcher.addPath(m_cfg->watchDirectory());

    m_running = true;
}

void FileMonitorService::stop() {
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

bool FileMonitorService::isRunning() const {
    return m_running;
}

void FileMonitorService::initialScan() {
    m_log->logEvent("Scanning directory", m_cfg->watchDirectory());

    QDir dir(m_cfg->watchDirectory());
    const QStringList exts = m_cfg->extensions();

    for (const auto &ext : exts) {
        m_log->logEvent("Using filter", ext);
    }

    const QFileInfoList files = dir.entryInfoList(exts, QDir::Files | QDir::NoSymLinks);
    m_log->logEvent("Files found", QString::number(files.size()));

    QHash<QString, QStringList> groups;
    for (const QFileInfo &fi : files) {
        groups[fi.completeBaseName()].append(fi.absoluteFilePath());
    }

    for (auto it = groups.constBegin(); it != groups.constEnd(); ++it) {
        const QString groupName = it.key();
        const QStringList paths = it.value();
        m_knownGroups.insert(groupName);

        QHash<QString, QByteArray> hashMap;
        for (const QString &path : paths) {
            QByteArray hash = m_hasher->fileHash(path, m_cfg->hashAlg());
            hashMap[path] = hash;
            m_backup->backupFile(path);
            m_watcher.addPath(path);
        }

        m_groupHashes[groupName] = hashMap;
        m_log->logEvent("Added group", groupName);
        emit fileAdded(groupName);
    }
}

void FileMonitorService::onDirectoryChanged(const QString &) {
    QDir dir(m_cfg->watchDirectory());
    const QStringList exts = m_cfg->extensions();
    const QFileInfoList files = dir.entryInfoList(exts, QDir::Files | QDir::NoSymLinks);

    QHash<QString, QStringList> groups;
    for (const QFileInfo &fi : files) {
        groups[fi.completeBaseName()].append(fi.absoluteFilePath());
    }

    for (auto it = groups.constBegin(); it != groups.constEnd(); ++it) {
        const QString groupName = it.key();
        const QStringList paths = it.value();

        bool isNewGroup = !m_knownGroups.contains(groupName);
        auto &hashMap = m_groupHashes[groupName];

        for (const QString &path : paths) {
            if (!hashMap.contains(path)) {
                QByteArray hash = m_hasher->fileHash(path, m_cfg->hashAlg());
                hashMap[path] = hash;
                m_backup->backupFile(path);
                m_watcher.addPath(path);
            }
        }

        m_groupHashes[groupName] = hashMap;

        if (isNewGroup) {
            m_knownGroups.insert(groupName);
            m_log->logEvent("New group added", groupName);
            emit fileAdded(groupName);
        }
    }
}


void FileMonitorService::onFileChanged(const QString& path) {
    QFileInfo fi(path);
    const QString groupName = fi.completeBaseName();
    if (!m_knownGroups.contains(groupName)) {
        return;
    }

    auto& hashMap = m_groupHashes[groupName];
    const QStringList paths = hashMap.keys();

    bool allDeleted = std::all_of(paths.begin(), paths.end(), [](const QString& p) {
        return !QFile::exists(p);
    });

    if (allDeleted) {
        m_log->logEvent("Group deleted", groupName);

        for (const QString& p : paths) {
            m_backup->restoreFile(p);
            m_watcher.addPath(p);
        }

        m_log->logEvent("Group restored", groupName);

        static QElapsedTimer deathTimer;
        if (!deathTimer.isValid() || deathTimer.elapsed() > 1000) {
            emit fileRestored("all");
            deathTimer.restart();
        }

        return;
    }

    bool modified = false;
    for (const QString& p : paths) {
        if (!QFile::exists(p)) {
            continue;
        }

        QByteArray newH = m_hasher->fileHash(p, m_cfg->hashAlg());
        if (newH != hashMap[p]) {
            hashMap[p] = newH;
            modified = true;
        }
    }

    if (modified) {
        for (const QString& p : paths) {
            m_backup->backupFile(p);
        }

        m_log->logEvent("Group modified", groupName);
        emit fileUpdated(groupName);
    }
}

