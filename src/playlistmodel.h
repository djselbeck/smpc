#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>

#include "mpdtrack.h"

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
public:
    explicit PlaylistModel(QObject *parent = 0);
        ~PlaylistModel(){
        if(m_entries){
                        qDeleteAll(*m_entries);
                        delete(m_entries);
                        m_entries = 0;
        }
                   }
        PlaylistModel(QList<MpdTrack*>* list,QObject *parent = 0);
    enum EntryRoles {
        NameRole = Qt::UserRole + 1,
        pathRole,
        filenameRole,
        titleRole,
        artistRole,
        albumRole,
        lengthRole,
        lengthFormatedRole,
        tracknoRole,
        yearRole,
        playingRole
    };

    Q_INVOKABLE MpdTrack* get(int index) {
        MpdTrack *retTrack;
        QQmlEngine::setObjectOwnership(retTrack,QQmlEngine::CppOwnership);
        return retTrack; }
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const;
    void setPlaying(int position,bool playing);
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);


private:
    QList<MpdTrack*>* m_entries;

signals:

public slots:
};

#endif // PLAYLISTMODEL_H
