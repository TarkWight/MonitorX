#ifndef IFILEMONITORSERVICE_HPP
#define IFILEMONITORSERVICE_HPP

#include <QObject>
#include <QString>

/**
 * @brief Interface for file monitoring service.
 *
 * Supports starting and stopping the monitoring process.
 * Emits signals when file groups are added, updated or restored.
 */
class IFileMonitorService : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;
    virtual ~IFileMonitorService() = default;

    /// Starts monitoring (if not already running)
    virtual void start() = 0;

    /// Stops monitoring (if running)
    virtual void stop() = 0;

    /// Returns whether monitoring is currently active
    virtual bool isRunning() const = 0;

signals:
    void fileAdded(const QString& groupName);
    void fileUpdated(const QString& groupName);
    void fileRestored(const QString& groupName);
};

#endif // IFILEMONITORSERVICE_HPP
