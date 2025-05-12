//===- MonitorX/src/di/DependencyInjection.cpp - DI Container Implementation -*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file implements the DI container that initializes and wires together
// all core components: logger, config manager, hasher, backup manager,
// monitor service, and view model.
//
//===-----------------------------------------------------------------------------------===//

#include "DependencyInjection.hpp"

#include <QCoreApplication>

DI::DI(const QString &configPath, QObject *parent) {
    m_logger = std::make_unique<LogManager>(
        QCoreApplication::applicationDirPath() + "/monitor.log", parent);

    m_hasher = std::make_unique<HashManager>(m_logger.get());

    m_backupManager = std::make_unique<BackupManager>(m_logger.get(), parent);

    m_configManager = std::make_unique<ConfigManager>(
        configPath, m_logger.get(), parent);
    m_configManager->load();

    m_monitorService = std::make_unique<FileMonitorService>(
        m_configManager.get(),
        m_backupManager.get(),
        m_hasher.get(),
        m_logger.get(),
        parent);

    m_monitorViewModel = std::make_unique<MonitorViewModel>(
        m_configManager.get(),
        m_monitorService.get(),
        m_logger.get(),
        parent);
}
