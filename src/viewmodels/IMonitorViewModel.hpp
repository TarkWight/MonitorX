#ifndef IMONITORVIEWMODEL_HPP
#define IMONITORVIEWMODEL_HPP

#include <QObject>
#include <QString>
#include <QAbstractListModel>

class IMonitorViewModel : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;
    virtual ~IMonitorViewModel() = default;

    virtual QString watchDirectory() const = 0;
    virtual int deaths() const = 0;
    virtual int saves() const = 0;
    virtual bool isRunning() const = 0;
    virtual QAbstractListModel* eventModel() const = 0;

public slots:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void setWatchDirectory(const QString& dir) = 0;
    virtual void openLogs() const = 0;

signals:
    void watchDirectoryChanged();
    void statsChanged();
    void runningChanged();
};

#endif // IMONITORVIEWMODEL_HPP
