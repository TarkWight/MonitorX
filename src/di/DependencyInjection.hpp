//===-- MonitorX/src/di/DependencyInjection.hpp - DI Container for MonitorX --*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file defines the DI (Dependency Injection) container for instantiating
// and managing all core services and their dependencies within the app.
//
//===-----------------------------------------------------------------------------------===//

#ifndef DEPENDENCYINJECTION_HPP
#define DEPENDENCYINJECTION_HPP

#include <memory>
#include <QString>

#include "ILogger.hpp"
#include "IHashManager.hpp"
#include "IBackupManager.hpp"
#include "IConfigManager.hpp"
#include "IFileMonitorService.hpp"

#include "LogManager.hpp"
#include "HashManager.hpp"
#include "BackupManager.hpp"
#include "ConfigManager.hpp"
#include "FileMonitorService.hpp"
#include "MonitorViewModel.hpp"

/// \brief Central dependency injection container for MonitorX.
class DI {
public:
    explicit DI(const QString &configPath, QObject *parent = nullptr);

    ILogger *logger() const { return m_logger.get(); }
    IHashManager *hasher() const { return m_hasher.get(); }
    IBackupManager *backupManager() const { return m_backupManager.get(); }
    IConfigManager *configManager() const { return m_configManager.get(); }
    IFileMonitorService *monitorService() const { return m_monitorService.get(); }
    MonitorViewModel *monitorViewModel() const { return m_monitorViewModel.get(); }

private:
    std::unique_ptr<LogManager>         m_logger;
    std::unique_ptr<HashManager>        m_hasher;
    std::unique_ptr<BackupManager>      m_backupManager;
    std::unique_ptr<ConfigManager>      m_configManager;
    std::unique_ptr<FileMonitorService> m_monitorService;
    std::unique_ptr<MonitorViewModel>   m_monitorViewModel;
};

#endif // DEPENDENCYINJECTION_HPP
