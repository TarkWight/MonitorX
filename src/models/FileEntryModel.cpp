#include "FileEntryModel.hpp"

FileEntryModel::FileEntryModel(QObject* parent)
    : QAbstractListModel(parent)
{}

int FileEntryModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_entries.size();
}

QVariant FileEntryModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_entries.size())
        return {};

    const FileEntry &e = m_entries.at(index.row());
    switch (role) {
    case GroupRole:     return e.groupName;
    case ActionRole:    return e.action;
    case TimestampRole: return e.timestamp.toString(Qt::ISODate);
    case Qt::DisplayRole:
        return QString("%1: %2 (%3)")
            .arg(e.groupName, e.action, e.timestamp.toString("hh:mm:ss"));
    default:
        return {};
    }
}

QHash<int,QByteArray> FileEntryModel::roleNames() const
{
    return {
        { GroupRole,     "groupName" },
        { ActionRole,    "action"    },
        { TimestampRole, "timestamp" }
    };
}

void FileEntryModel::addOrUpdateEntry(const FileEntry& entry)
{
    // ищем существующую запись
    for (int i = 0; i < m_entries.size(); ++i) {
        if (m_entries[i].groupName == entry.groupName) {
            beginMoveRows(QModelIndex(), i, i, QModelIndex(), 0);
            m_entries.move(i, 0);
            endMoveRows();
            m_entries[0].action    = entry.action;
            m_entries[0].timestamp = entry.timestamp;
            QModelIndex top = index(0, 0);
            emit dataChanged(top, top, { ActionRole, TimestampRole, Qt::DisplayRole });
            return;
        }
    }
    // вставляем новую запись
    beginInsertRows(QModelIndex(), 0, 0);
    m_entries.prepend(entry);
    endInsertRows();
}

void FileEntryModel::clear()
{
    if (m_entries.isEmpty())
        return;
    beginRemoveRows(QModelIndex(), 0, m_entries.size() - 1);
    m_entries.clear();
    endRemoveRows();
}
