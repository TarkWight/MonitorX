#ifndef HASHMANAGER_HPP
#define HASHMANAGER_HPP

#include <QString>
#include <QByteArray>
#include <QCryptographicHash>

/**
 * @brief Computes cryptographic hashes of files.
 */
class HashManager {
public:
    /**
     * @brief Computes the hash of the file at @a filePath using @a algorithm.
     * @return QByteArray containing the raw hash bytes (empty on error).
     */
    static QByteArray fileHash(const QString& filePath,
                               QCryptographicHash::Algorithm algorithm);
};

#endif // HASHMANAGER_HPP
