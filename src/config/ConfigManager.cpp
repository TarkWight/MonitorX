#include "ConfigManager.hpp"

#include <QJsonDocument>

ConfigManager::ConfigManager(const QString& configFilePath,
                             ILogger* logger,
                             QObject* parent)
    : IConfigManager(parent)
    , m_configPath(configFilePath)
    , m_logger(logger)
{}

bool ConfigManager::load()
{
    QFile f(m_configPath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (m_logger)
            m_logger->logEvent("Failed to open config file", m_configPath);
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isObject()) {
        if (m_logger)
            m_logger->logEvent("Invalid JSON format in config file", m_configPath);
        return false;
    }

    m_cfg = doc.object();
    if (m_logger)
        m_logger->logEvent("Config loaded", m_configPath);
    return true;
}

bool ConfigManager::save() const
{
    QFile f(m_configPath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        if (m_logger)
            m_logger->logEvent("Failed to save config file", m_configPath);
        return false;
    }

    QJsonDocument doc(m_cfg);
    f.write(doc.toJson(QJsonDocument::Indented));

    if (m_logger)
        m_logger->logEvent("Config saved", m_configPath);
    return true;
}

QString ConfigManager::watchDirectory() const
{
    return m_cfg.value("watchDirectory").toString();
}

QStringList ConfigManager::extensions() const
{
    QStringList exts;
    const QJsonArray arr = m_cfg.value("extensions").toArray();
    for (const auto& v : arr)
        exts << v.toString();
    return exts;
}

QString ConfigManager::backupDirectory() const
{
    return m_cfg.value("backupDirectory").toString();
}

QString ConfigManager::logFile() const
{
    return m_cfg.value("logFile").toString();
}

QCryptographicHash::Algorithm ConfigManager::hashAlg() const
{
    return algorithmFromString(m_cfg.value("hashAlgorithm").toString());
}

void ConfigManager::setWatchDirectory(const QString& dir)
{
    m_cfg["watchDirectory"] = dir;
    emit configChanged();
}

void ConfigManager::setExtensions(const QStringList& exts)
{
    QJsonArray arr;
    for (const auto& e : exts)
        arr.append(e);
    m_cfg["extensions"] = arr;
    emit configChanged();
}

void ConfigManager::setBackupDirectory(const QString& dir)
{
    m_cfg["backupDirectory"] = dir;
    emit configChanged();
}

void ConfigManager::setLogFile(const QString& file)
{
    m_cfg["logFile"] = file;
    emit configChanged();
}

void ConfigManager::setHashAlg(QCryptographicHash::Algorithm alg)
{
    m_cfg["hashAlgorithm"] = stringFromAlgorithm(alg);
    emit configChanged();
}

QCryptographicHash::Algorithm ConfigManager::algorithmFromString(const QString& s)
{
    if (s.compare("Md5", Qt::CaseInsensitive) == 0) return QCryptographicHash::Md5;
    if (s.compare("Sha1", Qt::CaseInsensitive) == 0) return QCryptographicHash::Sha1;
    if (s.compare("Sha256", Qt::CaseInsensitive) == 0) return QCryptographicHash::Sha256;
    if (s.compare("Sha512", Qt::CaseInsensitive) == 0) return QCryptographicHash::Sha512;
    return QCryptographicHash::Sha256; // default
}

QString ConfigManager::stringFromAlgorithm(QCryptographicHash::Algorithm alg)
{
    switch (alg) {
    case QCryptographicHash::Md5:    return "Md5";
    case QCryptographicHash::Sha1:   return "Sha1";
    case QCryptographicHash::Sha256: return "Sha256";
    case QCryptographicHash::Sha512: return "Sha512";
    default:                         return "Sha256";
    }
}
