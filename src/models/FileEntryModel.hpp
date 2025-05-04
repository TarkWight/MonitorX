#ifndef FILEENTRYMODEL_HPP
#define FILEENTRYMODEL_HPP

#include <QAbstractListModel>
#include <QDateTime>
#include <QVector>

/** Описание одной записи истории по группе сохранения */
struct FileEntry {
    QString   groupName;
    QString   action;
    QDateTime timestamp;
};

class FileEntryModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit FileEntryModel(QObject* parent = nullptr);

    enum Roles {
        GroupRole     = Qt::UserRole + 1,
        ActionRole,
        TimestampRole
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    /** Добавить или обновить запись по groupName */
    void addOrUpdateEntry(const FileEntry& entry);
    /** Очистить всю историю */
    void clear();

private:
    QVector<FileEntry> m_entries;
};

#endif // FILEENTRYMODEL_HPP
