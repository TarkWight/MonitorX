#ifndef FILEMONITORSERVICE_HPP
#define FILEMONITORSERVICE_HPP

#include <QObject>
#include <QFileSystemWatcher>
#include <QHash>
#include <QString>
#include <QByteArray>

class ConfigManager;
class BackupManager;
class LogManager;

class FileMonitorService : public QObject {
    Q_OBJECT

public:
    explicit FileMonitorService(ConfigManager* cfg, QObject* parent = nullptr);
    ~FileMonitorService() override;

    /// Запустить мониторинг
    void start();
    /// Остановить мониторинг
    void stop();
    /// Монитор запущен?
    bool isRunning() const { return m_running; }

signals:
    /// Новый файл найден и добавлен в бэкап
    void fileAdded(const QString& path);
    /// Файл изменился (басеап обновлён)
    void fileUpdated(const QString& path);
    /// Файл удалён и восстановлен из бэкапа
    void fileRestored(const QString& path);

private slots:
    void onDirectoryChanged(const QString& path);
    void onFileChanged(const QString& path);

private:
    void initialScan();
    void handleFileChange(const QString& path);
    void handleFileRemoval(const QString& path);

    ConfigManager*       m_cfg;
    BackupManager*       m_backup;
    LogManager*          m_log;
    QFileSystemWatcher   m_watcher;
    QHash<QString,QByteArray> m_hashes;  // путь → последний известный хеш
    bool                 m_running = false;
};

#endif // FILEMONITORSERVICE_HPP
