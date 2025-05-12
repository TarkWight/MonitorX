//===-- MonitorX/src/utils/HashManager.hpp - File Hashing Implementation --*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file defines the HashManager class, which implements the IHashManager interface.
// It computes cryptographic hashes of files and logs hashing activity via an ILogger
// instance.
//
//===--------------------------------------------------------------------------------===//

#ifndef HASHMANAGER_HPP
#define HASHMANAGER_HPP

#include "IHashManager.hpp"
#include "ILogger.hpp"

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QCryptographicHash>

class HashManager : public QObject, public IHashManager {
    Q_OBJECT

public:
    explicit HashManager(ILogger *logger, QObject *parent = nullptr);

    QByteArray fileHash(const QString &filePath,
                        QCryptographicHash::Algorithm algorithm) override;

private:
    ILogger *m_logger;
};

#endif // HASHMANAGER_HPP
