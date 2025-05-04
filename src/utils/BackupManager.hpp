#ifndef BACKUPMANAGER_HPP
#define BACKUPMANAGER_HPP

#include <QObject>
#include <QString>

/**
 * @brief Менеджер резервных копий файлов.
 *
 * Отвечает за создание резервных копий в заданной папке и восстановление из неё.
 * Перед использованием необходимо вызвать setBackupDir().
 *
 * Использует простую схему: имя файла → файл в backupDirectory.
 */
class BackupManager : public QObject {
    Q_OBJECT

public:
    explicit BackupManager(QObject* parent = nullptr);

    /**
     * @brief Установить (и создать, если нужно) папку для бэкапов.
     * @param dir Папка. Может быть абсолютной или относительной.
     */
    void setBackupDir(const QString& dir);

    /**
     * @brief Сделать резервную копию файла.
     * @param srcPath Полный путь к оригинальному файлу.
     */
    void backupFile(const QString& srcPath);

    /**
     * @brief Восстановить файл из резервной копии.
     * @param dstPath Полный путь, куда будет восстановлен файл.
     */
    void restoreFile(const QString& dstPath);

private:
    QString m_backupDir;
};

#endif // BACKUPMANAGER_HPP
