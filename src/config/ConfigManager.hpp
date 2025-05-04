#ifndef CONFIGMANAGER_HPP
#define CONFIGMANAGER_HPP

#include <QObject>
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <QJsonObject>

class ConfigManager : public QObject {
    Q_OBJECT

public:
    explicit ConfigManager(const QString& configFilePath, QObject* parent = nullptr);

    /// Загрузить настройки из JSON-файла. Возвращает false при ошибке.
    bool load();

    /// Сохранить текущее состояние в JSON-файл. Возвращает false при ошибке.
    bool save() const;

    // --- getters ---
    QString watchDirectory()   const;
    QStringList extensions()   const;
    QString backupDirectory()  const;
    QString logFile()          const;
    QCryptographicHash::Algorithm hashAlg() const;

    // --- setters (эти методы испускают configChanged()) ---
    void setWatchDirectory(const QString& dir);
    void setExtensions(const QStringList& exts);
    void setBackupDirectory(const QString& dir);
    void setLogFile(const QString& file);
    void setHashAlg(QCryptographicHash::Algorithm alg);

signals:
    /// Эмитится, когда поменялась любая опция
    void configChanged();

private:
    static QCryptographicHash::Algorithm algorithmFromString(const QString& s);
    static QString stringFromAlgorithm(QCryptographicHash::Algorithm alg);

    const QString  m_configPath;
    QJsonObject    m_cfg;
};
#endif // CONFIGMANAGER_HPP
