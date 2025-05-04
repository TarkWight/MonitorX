#include "ConfigManager.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>

ConfigManager::ConfigManager(const QString& configPath, QObject* parent)
    : QObject(parent)
    , m_configPath(configPath)
{}

bool ConfigManager::load()
{
    QFile f(m_configPath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    auto doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isObject())
        return false;

    m_cfg = doc.object();
    return true;
}

bool ConfigManager::save() const
{
    QFile f(m_configPath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return false;

    QJsonDocument doc(m_cfg);
    f.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

QString ConfigManager::watchDirectory() const
{
    return m_cfg.value("watchDirectory").toString();
}

QStringList ConfigManager::extensions() const
{
    QStringList exts;
    auto arr = m_cfg.value("extensions").toArray();
    for (auto v : arr)
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
    for (auto& e : exts) arr.append(e);
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
    if (s.compare("Md5", Qt::CaseInsensitive) == 0)
        return QCryptographicHash::Md5;
    if (s.compare("Sha1", Qt::CaseInsensitive) == 0)
        return QCryptographicHash::Sha1;
    if (s.compare("Sha256", Qt::CaseInsensitive) == 0)
        return QCryptographicHash::Sha256;
    if (s.compare("Sha512", Qt::CaseInsensitive) == 0)
        return QCryptographicHash::Sha512;
    // default
    return QCryptographicHash::Sha256;
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
