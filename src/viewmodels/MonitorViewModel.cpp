#include "MonitorViewModel.hpp"
#include "ConfigManager.hpp"
#include "FileMonitorService.hpp"
#include "FileEntryModel.hpp"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QDateTime>

MonitorViewModel::MonitorViewModel(const QString& configPath, QObject* parent)
    : QObject(parent)
    , m_cfg(new ConfigManager(configPath, this))
    , m_service(new FileMonitorService(m_cfg, this))
    , m_eventModel(new FileEntryModel(this))
{
    if (!m_cfg->load()) {
        qWarning("MonitorViewModel: cannot load config from %s",
                 qPrintable(configPath));
    }
    m_watchDirectory = m_cfg->watchDirectory();

    connect(m_service, &FileMonitorService::fileAdded,
            this, &MonitorViewModel::onFileAdded);
    connect(m_service, &FileMonitorService::fileUpdated,
            this, &MonitorViewModel::onFileUpdated);
    connect(m_service, &FileMonitorService::fileRestored,
            this, &MonitorViewModel::onFileRestored);

    connect(m_cfg, &ConfigManager::configChanged,
            this, &MonitorViewModel::onConfigChanged);
}

MonitorViewModel::~MonitorViewModel() = default;

QString MonitorViewModel::watchDirectory() const
{
    return m_watchDirectory;
}

int MonitorViewModel::deaths() const
{
    return m_deaths;
}

int MonitorViewModel::saves() const
{
    return m_saves;
}

bool MonitorViewModel::isRunning() const
{
    return m_running;
}

QAbstractListModel* MonitorViewModel::eventModel() const
{
    return m_eventModel;
}

void MonitorViewModel::start()
{
    if (m_running)
        return;

    m_deaths = 0;
    m_saves  = 0;
    emit statsChanged();

    m_eventModel->clear();
    m_service->start();
    setRunning(true);
}

void MonitorViewModel::stop()
{
    if (!m_running)
        return;

    m_service->stop();
    setRunning(false);
}

void MonitorViewModel::setWatchDirectory(const QString& dir)
{
    if (dir == m_watchDirectory)
        return;

    m_watchDirectory = dir;
    m_cfg->setWatchDirectory(dir);
    m_cfg->save();
    emit watchDirectoryChanged();

    if (m_running) {
        m_service->stop();
        m_service->start();
    }
}

void MonitorViewModel::openLogs() const
{
    const QString logPath = QCoreApplication::applicationDirPath()
    + "/" + m_cfg->logFile();
    QDesktopServices::openUrl(QUrl::fromLocalFile(logPath));
}

void MonitorViewModel::onFileAdded(const QString& groupName)
{
    ++m_saves;
    emit statsChanged();

    m_eventModel->addOrUpdateEntry({
        groupName,
        tr("Added"),
        QDateTime::currentDateTime()
    });
}

void MonitorViewModel::onFileUpdated(const QString& groupName)
{
    m_eventModel->addOrUpdateEntry({
        groupName,
        tr("Modified"),
        QDateTime::currentDateTime()
    });
}

void MonitorViewModel::onFileRestored(const QString& groupName)
{
    ++m_deaths;
    m_saves = 0;
    emit statsChanged();

    m_eventModel->addOrUpdateEntry({
        groupName,
        tr("Restored"),
        QDateTime::currentDateTime()
    });
}

void MonitorViewModel::onConfigChanged()
{
    m_watchDirectory = m_cfg->watchDirectory();
    emit watchDirectoryChanged();
}

void MonitorViewModel::setRunning(bool running)
{
    if (m_running == running)
        return;
    m_running = running;
    emit runningChanged();
}
