//===-- MonitorX/src/models/FileEntryModel.cpp - Event List Model Implementation -===//
//
//                     MonitorX — Save File Monitoring Tool
//
// This file contains the implementation of the FileEntryModel class,
// which provides a Qt-compatible model of recent save/restore events.
//
//===-----------------------------------------------------------------------------===//

#include "FileEntryModel.hpp"

FileEntryModel::FileEntryModel(QObject *parent)
    : QAbstractListModel(parent) {}

int FileEntryModel::rowCount(const QModelIndex & /*parent*/) const {
    return m_entries.size();
}

QVariant FileEntryModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size()) {
        return {};
    }

    const FileEntry &entry = m_entries.at(index.row());

    switch (role) {
    case GroupRole:
        return entry.GroupName;
    case ActionRole:
        return entry.Action;
    case TimestampRole:
        return entry.Timestamp.toString(Qt::ISODate);
    case Qt::DisplayRole:
        return QString("%1: %2 (%3)")
            .arg(entry.GroupName, entry.Action, entry.Timestamp.toString("hh:mm:ss"));
    default:
        return {};
    }
}

QHash<int, QByteArray> FileEntryModel::roleNames() const {
    return {
        {GroupRole, "groupName"},
        {ActionRole, "action"},
        {TimestampRole, "timestamp"}
    };
}

void FileEntryModel::addOrUpdateEntry(const FileEntry &entry) {
    // Check for existing entry and move it to top if found
    for (int i = 0; i < m_entries.size(); ++i) {
        if (m_entries[i].GroupName == entry.GroupName) {
            beginMoveRows(QModelIndex(), i, i, QModelIndex(), 0);
            m_entries.move(i, 0);
            endMoveRows();

            m_entries[0].Action = entry.Action;
            m_entries[0].Timestamp = entry.Timestamp;

            QModelIndex top = index(0, 0);
            emit dataChanged(top, top, {ActionRole, TimestampRole, Qt::DisplayRole});
            return;
        }
    }

    // Insert new entry at the top
    beginInsertRows(QModelIndex(), 0, 0);
    m_entries.prepend(entry);
    endInsertRows();
}

void FileEntryModel::clear() {
    if (m_entries.isEmpty()) {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, m_entries.size() - 1);
    m_entries.clear();
    endRemoveRows();
}
