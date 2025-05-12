//===--  MonitorX/src/servicesIFileMonitorService.hpp - Abstract File Monitor --*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file defines the interface IFileMonitorService, which provides an
// abstract API for starting/stopping directory monitoring and emitting
// signals on save group changes.
//
//===-------------------------------------------------------------------------------------===//

#ifndef IFILEMONITORSERVICE_HPP
#define IFILEMONITORSERVICE_HPP

#include <QObject>
#include <QString>


class IFileMonitorService : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;
    ~IFileMonitorService() override = default;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isRunning() const = 0;

signals:
    void fileAdded(const QString &groupName);
    void fileUpdated(const QString &groupName);
    void fileRestored(const QString &groupName);
};

#endif // IFILEMONITORSERVICE_HPP
