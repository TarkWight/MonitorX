#ifndef FILEMONITORSERVICE_HPP
#define FILEMONITORSERVICE_HPP

#include <QObject>
#include <QFileSystemWatcher>
#include <QHash>
#include <QSet>
#include <QString>
#include <QByteArray>

class ConfigManager;
class BackupManager;
class LogManager;

/**
 * Мониторит папку сохранений, группируя файлы по базовому имени.
 * Сигналы эмитятся на уровне группы:
 *  - fileAdded(groupName)
 *  - fileUpdated(groupName)
 *  - fileRestored(groupName)
 */
class FileMonitorService : public QObject {
    Q_OBJECT

public:
    explicit FileMonitorService(ConfigManager* cfg, QObject* parent = nullptr);
    ~FileMonitorService() override;

    void start();
    void stop();
    bool isRunning() const { return m_running; }

signals:
    void fileAdded(const QString& groupName);
    void fileUpdated(const QString& groupName);
    void fileRestored(const QString& groupName);

private slots:
    void onDirectoryChanged(const QString& /*path*/);
    void onFileChanged(const QString& path);

private:
    void initialScan();

    ConfigManager* m_cfg;
    BackupManager* m_backup;
    LogManager*    m_log;
    QFileSystemWatcher m_watcher;

    // groupName → ( filePath → lastHash )
    QHash<QString, QHash<QString, QByteArray>> m_groupHashes;
    QSet<QString> m_knownGroups;

    bool m_running = false;
};

#endif // FILEMONITORSERVICE_HPP
