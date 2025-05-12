//===-- MonitorX/src/viewmodels/MonitorViewModel.cpp - Monitor ViewModel Implementation -*- C++ -*-===//
//
// Part of the MonitorX savegame monitoring application.
//
// This file implements the MonitorViewModel class, which manages presentation logic and statistics
// for the file monitoring UI. It coordinates configuration, service control, and log interactions.
//
//===----------------------------------------------------------------------------------------------===//

#include "MonitorViewModel.hpp"
#include "FileEntryModel.hpp"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QDateTime>

MonitorViewModel::MonitorViewModel(IConfigManager *config,
                                   IFileMonitorService *service,
                                   ILogger *logger,
                                   QObject *parent)
    : IMonitorViewModel(parent),
    m_cfg(config),
    m_service(service),
    m_log(logger),
    m_eventModel(new FileEntryModel(this)) {
    m_watchDirectory = m_cfg->watchDirectory();

    connect(m_service, &IFileMonitorService::fileAdded,
            this, &MonitorViewModel::onFileAdded);
    connect(m_service, &IFileMonitorService::fileUpdated,
            this, &MonitorViewModel::onFileUpdated);
    connect(m_service, &IFileMonitorService::fileRestored,
            this, &MonitorViewModel::onFileRestored);

    connect(m_cfg, &IConfigManager::configChanged,
            this, &MonitorViewModel::onConfigChanged);
    connect(m_cfg, &QObject::destroyed,
            this, &MonitorViewModel::stop);
}

MonitorViewModel::~MonitorViewModel() = default;

QString MonitorViewModel::watchDirectory() const {
    return m_watchDirectory;
}

int MonitorViewModel::deaths() const {
    return m_deaths;
}

int MonitorViewModel::saves() const {
    return m_saves;
}

bool MonitorViewModel::isRunning() const {
    return m_running;
}

QAbstractListModel *MonitorViewModel::eventModel() const {
    return m_eventModel;
}

void MonitorViewModel::start() {
    if (m_running) {
        return;
    }

    m_deaths = 0;
    m_saves = 0;
    emit statsChanged();

    m_eventModel->clear();
    m_service->start();
    setRunning(true);
}

void MonitorViewModel::stop() {
    if (!m_running) {
        return;
    }

    m_service->stop();
    setRunning(false);
}

void MonitorViewModel::setWatchDirectory(const QString &dir) {
    if (dir == m_watchDirectory) {
        return;
    }

    m_watchDirectory = dir;
    m_cfg->setWatchDirectory(dir);
    m_cfg->save();
    emit watchDirectoryChanged();

    if (m_running) {
        m_service->stop();
        m_service->start();
    }
}

void MonitorViewModel::openLogs() const {
    const QString logPath = QCoreApplication::applicationDirPath() + "/" + m_cfg->logFile();
    QDesktopServices::openUrl(QUrl::fromLocalFile(logPath));
}

void MonitorViewModel::onFileAdded(const QString &groupName) {
    ++m_saves;
    emit statsChanged();

    m_eventModel->addOrUpdateEntry({
        groupName,
        tr("Added"),
        QDateTime::currentDateTime()
    });
}

void MonitorViewModel::onFileUpdated(const QString &groupName) {
    m_eventModel->addOrUpdateEntry({
        groupName,
        tr("Modified"),
        QDateTime::currentDateTime()
    });
}

void MonitorViewModel::onFileRestored(const QString &groupName) {
    if (groupName != "all") {
        return;
    }

    ++m_deaths;
    m_saves = 0;
    emit statsChanged();

    m_eventModel->clear();
    m_eventModel->addOrUpdateEntry({
        tr("All saves restored"),
        tr("Death detected"),
        QDateTime::currentDateTime()
    });
}

void MonitorViewModel::onConfigChanged() {
    m_watchDirectory = m_cfg->watchDirectory();
    emit watchDirectoryChanged();
}

void MonitorViewModel::setRunning(bool running) {
    if (m_running == running) {
        return;
    }

    m_running = running;
    emit runningChanged();
}
