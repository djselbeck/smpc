#ifndef DATABASEFILLJOB_H
#define DATABASEFILLJOB_H

#include <QObject>

#include "mpdartist.h"
#include "mpdalbum.h"
#include "lastfmalbumprovider.h"
#include "albuminformation.h"

class DatabaseFillJob : public QObject
{
    Q_OBJECT
public:
    DatabaseFillJob();

public slots:
    void startFilling(QMap<MpdArtist*, QList<MpdAlbum*>* > *map);

private:
    bool mRunning;
    QMap<MpdArtist*, QList<MpdAlbum*>* > *mMap;
    QMap<MpdArtist*, QList<MpdAlbum*>* >::iterator mCurrentArtistPosition;
    QList<MpdAlbum*>::iterator mCurrentAlbumPosition;
    LastFMAlbumProvider *mCurrentProvider;

private slots:

    void albumFinished(AlbumInformation*);

};

#endif // DATABASEFILLJOB_H
