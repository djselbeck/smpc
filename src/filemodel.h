#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>
#include "mpdfileentry.h"

class FileModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
public:
    explicit FileModel(QObject *parent = 0);
    ~FileModel(){CommonDebug("Artistmodel destroyed");
                   for(int i=0;i<m_entries->length();i++)
                   {
                       delete(m_entries->at(i));
                   }
                   delete(m_entries);
                  }

        FileModel(QList<MpdFileEntry*>* list,QObject *parent = 0);
    enum EntryRoles {
        NameRole = Qt::UserRole + 1,
        SectionRole,
        isDirectoryRole,
        isFileRole,
        isPlaylistRole,
        prePathRole,
        pathRole,
        titleRole,
        artistRole,
        albumRole,
        lengthRole,
        tracknoRole,
        yearRole,
    };

    Q_INVOKABLE MpdFileEntry* get(int index) {  return m_entries->at(index); }
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const;


private:
    QList<MpdFileEntry*>* m_entries;

signals:

public slots:
};

#endif // FILEMODEL_H
