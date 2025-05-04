#include "HashManager.hpp"
#include <QFile>
#include <QCryptographicHash>

QByteArray HashManager::fileHash(const QString& filePath,
                                 QCryptographicHash::Algorithm algorithm)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return {};

    QCryptographicHash hasher(algorithm);
    constexpr qint64 bufferSize = 8192;
    while (!file.atEnd()) {
        QByteArray chunk = file.read(bufferSize);
        hasher.addData(chunk);
    }
    return hasher.result();
}
