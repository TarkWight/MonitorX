#ifndef HASHMANAGER_HPP
#define HASHMANAGER_HPP

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QCryptographicHash>

#include "ILogger.hpp"
#include "IHashManager.hpp"

/**
 * @brief Computes cryptographic hashes of files and logs results.
 */
class HashManager : public QObject, public IHashManager {
    Q_OBJECT

public:
    explicit HashManager(ILogger* logger, QObject* parent = nullptr);

    QByteArray fileHash(const QString& filePath,
                        QCryptographicHash::Algorithm algorithm) override;

private:
    ILogger* m_logger;
};

#endif // HASHMANAGER_HPP
