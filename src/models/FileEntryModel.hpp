//===-- MonitorX/src/models/FileEntryModel.hpp - UI Event List Model ---*- C++ -*-===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file defines the FileEntryModel class, a Qt list model used to display
// save events (add, restore, modify) in the UI. Each row represents an entry
// with a group name, action description, and timestamp.
//
//===-----------------------------------------------------------------------------===//

#ifndef FILEENTRYMODEL_HPP
#define FILEENTRYMODEL_HPP

#include <QAbstractListModel>
#include <QDateTime>
#include <QVector>

struct FileEntry {
    QString GroupName;
    QString Action;
    QDateTime Timestamp;
};

class FileEntryModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit FileEntryModel(QObject *parent = nullptr);

    enum Roles {
        GroupRole = Qt::UserRole + 1,
        ActionRole,
        TimestampRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addOrUpdateEntry(const FileEntry &entry);
    void clear();

private:
    QVector<FileEntry> m_entries;
};

#endif // FILEENTRYMODEL_HPP
