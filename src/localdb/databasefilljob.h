#ifndef DATABASEFILLJOB_H
#define DATABASEFILLJOB_H

#include <QObject>

#include <mpd/mpdartist.h>
#include <mpd/mpdalbum.h>
#include <localdb/lastfmalbumprovider.h>
#include <localdb/albuminformation.h>

class DatabaseFillJob : public QObject
{
    Q_OBJECT
public:
    DatabaseFillJob();

public slots:
    void startFilling(QMap<MpdArtist*, QList<MpdAlbum*>* > *map);

signals:
    void albumReady(AlbumInformation *info);

private:
    bool mRunning;
    QMap<MpdArtist*, QList<MpdAlbum*>* > *mMap;
    QMap<MpdArtist*, QList<MpdAlbum*>* >::iterator mCurrentArtistPosition;
    QList<MpdAlbum*>::iterator mCurrentAlbumPosition;
    LastFMAlbumProvider *mCurrentProvider;

    // Imagedatabase which got data inserted

private slots:

    void albumFinished(AlbumInformation*);

};

#endif // DATABASEFILLJOB_H
