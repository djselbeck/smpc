#ifndef ALBUMMODEL_H
#define ALBUMMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>

#include <localdb/imagedatabase.h>
#include <localdb/imagedownloader.h>
#include <mpd/mpdalbum.h>


class AlbumModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
public:
    explicit AlbumModel(QObject *parent = 0);
    AlbumModel(QList<MpdAlbum*> *list, ImageDatabase *db, QString lastfmArtSize, QObject *parent = 0);
    ~AlbumModel();

    Q_INVOKABLE MpdAlbum* get(int index);
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const;

    enum EntryRoles {
        AlbumRole = Qt::UserRole + 1,
        SectionRole,
        ArtistRole,
        AlbumCleandRole,
        AlbumImageRole
    };

private:
    QList<MpdAlbum*> *mEntries;

    // Imagedatabase for image storage/retrieval
    ImageDatabase *mDB;
    // Synchron downloader class to limit network usage
    ImageDownloader *mDownloader;

signals:
    void requestAlbumInformation(MpdAlbum album) const;
    void requestDBEnter(AlbumInformation *info);

public slots:
    void albumInformationReady(AlbumInformation *info);
    void albumEntered(QString albumName);

};

#endif // ALBUMMODEL_H
