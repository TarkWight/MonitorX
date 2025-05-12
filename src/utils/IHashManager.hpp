//===-- MonitorX/src/utils/IHashManager.hpp - Hashing Interface --*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file defines the IHashManager interface, responsible for computing
// cryptographic hashes of files using selected algorithms.
//
//===-----------------------------------------------------------------------===//

#ifndef IHASHMANAGER_H
#define IHASHMANAGER_H

#include <QByteArray>
#include <QCryptographicHash>
#include <QString>

class IHashManager {
public:
    virtual ~IHashManager() = default;

    virtual QByteArray fileHash(const QString &filePath,
                                QCryptographicHash::Algorithm algorithm) = 0;
};

#endif // IHASHMANAGER_H
