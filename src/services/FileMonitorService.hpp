#ifndef FILEMONITORSERVICE_HPP
#define FILEMONITORSERVICE_HPP

#include "IFileMonitorService.hpp"
#include "IConfigManager.hpp"
#include "IBackupManager.hpp"
#include "IHashManager.hpp"
#include "ILogger.hpp"

#include <QFileSystemWatcher>
#include <QHash>
#include <QSet>

/**
 * @brief Implementation of file monitoring service.
 */
class FileMonitorService : public IFileMonitorService {
    Q_OBJECT

public:
    explicit FileMonitorService(IConfigManager* cfg,
                                IBackupManager* backup,
                                IHashManager* hasher,
                                ILogger* logger,
                                QObject* parent = nullptr);
    ~FileMonitorService() override;

    void start() override;
    void stop() override;
    bool isRunning() const override;

private slots:
    void onDirectoryChanged(const QString& path);
    void onFileChanged(const QString& path);

private:
    void initialScan();

    IConfigManager* m_cfg;
    IBackupManager* m_backup;
    IHashManager*   m_hasher;
    ILogger*        m_log;

    QFileSystemWatcher m_watcher;

    QHash<QString, QHash<QString, QByteArray>> m_groupHashes;
    QSet<QString> m_knownGroups;
    bool m_running = false;
};

#endif // FILEMONITORSERVICE_HPP
