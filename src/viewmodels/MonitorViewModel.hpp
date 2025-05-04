# ifndef FILEMONITORVIEWMODEL_HPP
#define FILEMONITORVIEWMODEL_HPP

#include <QObject>
#include <QString>
#include <QAbstractListModel>

class ConfigManager;
class FileMonitorService;
class FileEntryModel;

class MonitorViewModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString watchDirectory   READ watchDirectory   NOTIFY watchDirectoryChanged)
    Q_PROPERTY(int     deaths           READ deaths           NOTIFY statsChanged)
    Q_PROPERTY(int     saves            READ saves            NOTIFY statsChanged)
    Q_PROPERTY(bool    running          READ isRunning        NOTIFY runningChanged)
    Q_PROPERTY(QAbstractListModel* eventModel READ eventModel CONSTANT)

public:
    explicit MonitorViewModel(const QString& configPath, QObject* parent = nullptr);
    ~MonitorViewModel() override;

    /// --- getters ---
    QString               watchDirectory() const;
    int                   deaths()         const;
    int                   saves()          const;
    bool                  isRunning()      const;
    QAbstractListModel*   eventModel()     const;

public slots:
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void setWatchDirectory(const QString& dir);
    Q_INVOKABLE void openLogs() const;

signals:
    void watchDirectoryChanged();
    void statsChanged();
    void runningChanged();

private slots:
    void onFileAdded(const QString& groupName);
    void onFileUpdated(const QString& groupName);
    void onFileRestored(const QString& groupName);
    void onConfigChanged();

private:
    void setRunning(bool running);

    ConfigManager*       m_cfg;
    FileMonitorService*  m_service;
    FileEntryModel*      m_eventModel;

    QString              m_watchDirectory;
    int                  m_deaths   = 0;
    int                  m_saves    = 0;
    bool                 m_running  = false;
};

#endif // FILEMONITORVIEWMODEL_HPP
