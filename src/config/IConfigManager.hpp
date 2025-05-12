//===-- MonitorX/src/config/IConfigManager.hpp - Configuration Interface ----*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file defines the interface IConfigManager, which provides access to
// configuration settings like watch paths, file filters, hashing algorithm,
// and log file location. It supports persistence and change notifications.
//
//===----------------------------------------------------------------------------------===//

#ifndef ICONFIGMANAGER_HPP
#define ICONFIGMANAGER_HPP

#include <QObject>
#include <QString>
#include <QStringList>
#include <QCryptographicHash>

class IConfigManager : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;
    virtual ~IConfigManager() = default;

    virtual bool load() = 0;
    virtual bool save() const = 0;

    virtual QString watchDirectory() const = 0;
    virtual QStringList extensions() const = 0;
    virtual QString backupDirectory() const = 0;
    virtual QString logFile() const = 0;
    virtual QCryptographicHash::Algorithm hashAlg() const = 0;

    virtual void setWatchDirectory(const QString &dir) = 0;
    virtual void setExtensions(const QStringList &exts) = 0;
    virtual void setBackupDirectory(const QString &dir) = 0;
    virtual void setLogFile(const QString &file) = 0;
    virtual void setHashAlg(QCryptographicHash::Algorithm alg) = 0;

signals:
    void configChanged();
};

#endif // ICONFIGMANAGER_HPP
