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

/**
 * @brief Dependency Injection container
 */
class DI {
public:
    explicit DI(const QString& configPath, QObject* parent = nullptr);

    ILogger* logger() const { return m_logger.get(); }
    IHashManager* hasher() const { return m_hasher.get(); }
    IBackupManager* backupManager() const { return m_backupManager.get(); }
    IConfigManager* configManager() const { return m_configManager.get(); }
    IFileMonitorService* monitorService() const { return m_monitorService.get(); }
    MonitorViewModel* monitorViewModel() const { return m_monitorViewModel.get(); }

private:
    std::unique_ptr<LogManager>         m_logger;
    std::unique_ptr<HashManager>        m_hasher;
    std::unique_ptr<BackupManager>      m_backupManager;
    std::unique_ptr<ConfigManager>      m_configManager;
    std::unique_ptr<FileMonitorService> m_monitorService;
    std::unique_ptr<MonitorViewModel>   m_monitorViewModel;
};

#endif // DEPENDENCYINJECTION_HPP
