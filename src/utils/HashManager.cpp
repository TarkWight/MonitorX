//===-- MonitorX/src/utils/HashManager.cpp - File Hashing Implementation --*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file contains the implementation of HashManager, which computes cryptographic
// hashes for files and logs operations via ILogger.
//
//===--------------------------------------------------------------------------------===//

#include "HashManager.hpp"
#include <QFile>

HashManager::HashManager(ILogger *logger, QObject *parent)
    : QObject(parent), m_logger(logger) {}

QByteArray HashManager::fileHash(const QString &filePath,
                                 QCryptographicHash::Algorithm algorithm) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        if (m_logger) {
            m_logger->logEvent("Hash FAILED (cannot open)", filePath);
        }
        return {};
    }

    QCryptographicHash hasher(algorithm);
    constexpr qint64 BufferSize = 8192;

    while (!file.atEnd()) {
        QByteArray chunk = file.read(BufferSize);
        hasher.addData(chunk);
    }

    QByteArray result = hasher.result();

    if (m_logger) {
        m_logger->logEvent("Hash calculated", filePath);
    }

    return result;
}
