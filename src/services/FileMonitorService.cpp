#include "FileMonitorService.hpp"
#include "ConfigManager.hpp"
#include "HashManager.hpp"
#include "BackupManager.hpp"
#include "LogManager.hpp"

#include <QDir>
#include <QFileInfo>
#include <QDateTime>

namespace fs = std::filesystem;

FileMonitorService::FileMonitorService(ConfigManager* cfg, QObject* parent)
    : QObject(parent)
    , m_cfg(cfg)
    , m_backup(new BackupManager(this))
    , m_log(new LogManager(cfg->logFile(), this))
{
    // Сигналы от watcher
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged,
            this, &FileMonitorService::onDirectoryChanged);
    connect(&m_watcher, &QFileSystemWatcher::fileChanged,
            this, &FileMonitorService::onFileChanged);

    // Перезапуск при смене конфига
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
    if (m_running)
        return;

    m_hashes.clear();
    m_watcher.removePaths(m_watcher.directories());
    m_watcher.removePaths(m_watcher.files());

    initialScan();
    m_watcher.addPath(m_cfg->watchDirectory());

    m_running = true;
}

void FileMonitorService::stop()
{
    if (!m_running)
        return;

    m_watcher.removePaths(m_watcher.directories());
    m_watcher.removePaths(m_watcher.files());
    m_hashes.clear();
    m_running = false;
}

void FileMonitorService::initialScan()
{
    QDir dir(m_cfg->watchDirectory());
    const QStringList exts = m_cfg->extensions();

    const QFileInfoList fileList = dir.entryInfoList(exts, QDir::Files | QDir::NoSymLinks);
    for (const QFileInfo &fi : fileList) {
        const QString path = fi.absoluteFilePath();
        QByteArray h = HashManager::fileHash(path, m_cfg->hashAlg());
        m_hashes[path] = h;
        m_backup->backupFile(path);
        m_log->logEvent("Added", path);
        emit fileAdded(path);
        m_watcher.addPath(path);
    }
}

void FileMonitorService::onDirectoryChanged(const QString &)
{
    QDir dir(m_cfg->watchDirectory());
    const QStringList exts = m_cfg->extensions();

    const QFileInfoList fileList = dir.entryInfoList(exts, QDir::Files | QDir::NoSymLinks);
    for (const QFileInfo &fi : fileList) {
        const QString path = fi.absoluteFilePath();
        if (!m_hashes.contains(path)) {
            QByteArray h = HashManager::fileHash(path, m_cfg->hashAlg());
            m_hashes[path] = h;
            m_backup->backupFile(path);
            m_log->logEvent("Added", path);
            emit fileAdded(path);
            m_watcher.addPath(path);
        }
    }
}


void FileMonitorService::onFileChanged(const QString& path)
{
    QFileInfo fi(path);
    if (!fi.exists()) {
        handleFileRemoval(path);
    } else {
        handleFileChange(path);
    }
}

void FileMonitorService::handleFileChange(const QString& path)
{
    QByteArray newHash = HashManager::fileHash(path, m_cfg->hashAlg());
    const QByteArray oldHash = m_hashes.value(path);
    if (newHash != oldHash) {
        m_hashes[path] = newHash;
        m_backup->backupFile(path);
        m_log->logEvent("Modified", path);
        emit fileUpdated(path);
    }
    // Продолжаем следить — QFileSystemWatcher не удаляет путь при изменении
}

void FileMonitorService::handleFileRemoval(const QString& path)
{
    m_log->logEvent("Deleted", path);
    m_backup->restoreFile(path);
    m_log->logEvent("Restored", path);
    emit fileRestored(path);
    // восстанавливая, мы заново создаём файл, но watcher сбросит этот путь,
    // поэтому:
    m_watcher.addPath(path);
}
