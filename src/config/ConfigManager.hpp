//===- MonitorX/src/config/ConfigManager.hpp - Configuration Implementation -*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file declares the ConfigManager class, which implements IConfigManager.
// It provides logic for reading, saving, and notifying configuration updates.
//
//===----------------------------------------------------------------------------------===//

#ifndef CONFIGMANAGER_HPP
#define CONFIGMANAGER_HPP

#include "IConfigManager.hpp"
#include "ILogger.hpp"

#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QDir>
#include <QFile>

class ConfigManager : public IConfigManager {
    Q_OBJECT

public:
    explicit ConfigManager(const QString &configFilePath,
                           ILogger *logger,
                           QObject *parent = nullptr);

    bool load() override;
    bool save() const override;

    QString watchDirectory() const override;
    QStringList extensions() const override;
    QString backupDirectory() const override;
    QString logFile() const override;
    QCryptographicHash::Algorithm hashAlg() const override;

    void setWatchDirectory(const QString &dir) override;
    void setExtensions(const QStringList &exts) override;
    void setBackupDirectory(const QString &dir) override;
    void setLogFile(const QString &file) override;
    void setHashAlg(QCryptographicHash::Algorithm alg) override;

private:
    static QCryptographicHash::Algorithm algorithmFromString(const QString &s);
    static QString stringFromAlgorithm(QCryptographicHash::Algorithm alg);

    QString m_configPath;
    QJsonObject m_cfg;
    ILogger *m_logger = nullptr;
};

#endif // CONFIGMANAGER_HPP
