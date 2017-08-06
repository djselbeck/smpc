#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>

#include <mpd/mpdtrack.h>
#include <mpd/mpdcommon.h>

#include <localdb/imagedatabase.h>

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
public:
    explicit PlaylistModel(QObject *parent = 0);
    PlaylistModel(ImageDatabase *db, QObject *parent = 0);
    ~PlaylistModel();

    Q_INVOKABLE MpdTrack* get(int index);
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const;
    void setPlaying(quint32 position,bool playing);
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
        playingRole,
        sectionRole,
        sectionImageURLRole,
        trackmbidRole,
        albummbidRole,
        artistmbidRole,
    };

private:
    QList<MpdTrack*>* mEntries;
    ImageDatabase *mDB;

    MPD_PLAYBACK_STATE mPlaybackState;
    quint32 mTrackNo;

signals:
    void clearModel();

public slots:
    void receiveNewTrackList(QList<MpdTrack*>* tracks);

    void onTrackNoChanged(quint32 trackNo);
    void onPlaybackStateChanged(MPD_PLAYBACK_STATE state);
};

#endif // PLAYLISTMODEL_H
