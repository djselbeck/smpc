#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>

#include <mpd/mpdtrack.h>

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
public:
    explicit PlaylistModel(QObject *parent = 0);
    PlaylistModel(QList<MpdTrack*>* list,QObject *parent = 0);
    ~PlaylistModel();

    Q_INVOKABLE MpdTrack* get(int index);
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const;
    void setPlaying(int position,bool playing);
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

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

private:
    QList<MpdTrack*>* mEntries;

signals:

public slots:
};

#endif // PLAYLISTMODEL_H
