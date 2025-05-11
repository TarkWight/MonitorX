#ifndef MONITORVIEWMODEL_HPP
#define MONITORVIEWMODEL_HPP

#include "IMonitorViewModel.hpp"
#include "IConfigManager.hpp"
#include "IFileMonitorService.hpp"
#include "ILogger.hpp"

#include <QAbstractListModel>

class FileEntryModel;

class MonitorViewModel : public IMonitorViewModel {
    Q_OBJECT

    Q_PROPERTY(QString watchDirectory   READ watchDirectory   NOTIFY watchDirectoryChanged)
    Q_PROPERTY(int     deaths           READ deaths           NOTIFY statsChanged)
    Q_PROPERTY(int     saves            READ saves            NOTIFY statsChanged)
    Q_PROPERTY(bool    running          READ isRunning        NOTIFY runningChanged)
    Q_PROPERTY(QAbstractListModel* eventModel READ eventModel CONSTANT)

public:
    explicit MonitorViewModel(IConfigManager* config,
                              IFileMonitorService* service,
                              ILogger* logger,
                              QObject* parent = nullptr);
    ~MonitorViewModel() override;

    QString watchDirectory() const override;
    int deaths() const override;
    int saves() const override;
    bool isRunning() const override;
    QAbstractListModel* eventModel() const override;

public slots:
    void start() override;
    void stop() override;
    void setWatchDirectory(const QString& dir) override;
    void openLogs() const override;

private slots:
    void onFileAdded(const QString& groupName);
    void onFileUpdated(const QString& groupName);
    void onFileRestored(const QString& groupName);
    void onConfigChanged();

private:
    void setRunning(bool running);

    IConfigManager*      m_cfg;
    IFileMonitorService* m_service;
    ILogger*             m_log;
    FileEntryModel*      m_eventModel;

    QString              m_watchDirectory;
    int                  m_deaths = 0;
    int                  m_saves = 0;
    bool                 m_running = false;
};

#endif // MONITORVIEWMODEL_HPP
