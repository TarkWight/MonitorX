#ifndef IHASHMANAGER_H
#define IHASHMANAGER_H

#include <QString>
#include <QByteArray>
#include <QCryptographicHash>

/**
 * @brief Interface for hashing files.
 */
class IHashManager {
public:
    virtual ~IHashManager() = default;

    /**
     * @brief Computes the hash of a file.
     * @return QByteArray with hash (empty if error).
     */
    virtual QByteArray fileHash(const QString& filePath,
                                QCryptographicHash::Algorithm algorithm) = 0;
};

#endif // IHASHMANAGER_H
