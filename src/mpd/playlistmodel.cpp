#include "playlistmodel.h"

#include <QDebug>

PlaylistModel::PlaylistModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

PlaylistModel::PlaylistModel(ImageDatabase *db, QObject *parent) : QAbstractListModel(parent)
{
    mDB = db;
    mEntries = 0;
    mPlaybackState = MPD_STOP;
    mTrackNo = 0;
}

PlaylistModel::~PlaylistModel(){
    if(mEntries){
        qDebug() << "freeing " << mEntries->size() << " elements with size of: " << sizeof(MpdTrack)*mEntries->size() << " bytes";
        qDeleteAll(*mEntries);
        mEntries->clear();
        delete(mEntries);
        mEntries = 0;
    }
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if ( Q_UNLIKELY(index.row() < 0 || index.row() > rowCount())) {
        return QVariant(QVariant::Invalid);
    }
    if(role==NameRole)
    {
        return mEntries->at(index.row())->getName();
    }
    else if(role==pathRole)
        return mEntries->at(index.row())->getFileUri();
    else if(role==filenameRole)
        return mEntries->at(index.row())->getFileName();
    else if(role==titleRole)
        return mEntries->at(index.row())->getTitle();
    else if(role==sectionRole) {
        MpdTrack  *tmpTrack = mEntries->at(index.row());
        QString album = tmpTrack->getAlbum();
        QString albumartist = tmpTrack->getAlbumArtist();
        QString artist = tmpTrack->getArtist();

        QString sectionString;
        if ( albumartist == "" ) {
            sectionString = artist + '|' + album;
        } else {
            sectionString = albumartist + '|' + album;
        }
        return sectionString;
    }
    else if(role==artistRole)
        return mEntries->at(index.row())->getArtist();
    else if(role==albumRole)
        return mEntries->at(index.row())->getAlbum();
    else if(role==lengthRole)
        return mEntries->at(index.row())->getLength();
    else if(role==lengthFormatedRole)
        return mEntries->at(index.row())->getLengthFormated();
    else if(role==tracknoRole)
        return mEntries->at(index.row())->getTrackNr();
    else if(role==yearRole)
        return mEntries->at(index.row())->getYear();
    else if(role==playingRole)
        return mEntries->at(index.row())->getPlaying();
    else if(role==trackmbidRole)
        return mEntries->at(index.row())->getTrackMBID();
    else if(role==albummbidRole)
        return mEntries->at(index.row())->getAlbumMBID();
    else if(role==artistmbidRole)
        return mEntries->at(index.row())->getArtistMBID();
    else if ( role== sectionImageURLRole ) {
        MpdTrack *track = mEntries->at(index.row());
        QString album = track->getAlbum();
        QString artist = track->getArtist();
        if ( artist != "" ) {
            int imageID = mDB->imageIDFromAlbumArtist(album,artist);
            // No image found return dummy url
            if ( imageID == -1 ) {
                // Start image retrieval
                qDebug() << "returning dummy image for album: " << album;
                //emit requestAlbumInformation(*album);
                // Return dummy for the time being
                return DUMMY_ALBUMIMAGE;
            } else if (imageID == -2 ) {
                // Try getting album art for album with out artist (think samplers)
                imageID = mDB->imageIDFromAlbum(album);
                if ( imageID >= 0 ) {
                    QString url = "image://imagedbprovider/albumid/" + QString::number(imageID);
                    return url;
                }
                qDebug() << "returning dummy image for blacklisted album: " << album;
                return DUMMY_ALBUMIMAGE;
            } else {
                qDebug() << "returning database image for album: " << album;
                QString url = "image://imagedbprovider/albumid/" + QString::number(imageID);
                return url;
            }
        }
        else {
            int imageID = mDB->imageIDFromAlbum(album);

            // No image found return dummy url
            if ( imageID == -1 ) {
                // Start image retrieval
                qDebug() << "returning dummy image for album: " << album;
                // Return dummy for the time being
                return DUMMY_ALBUMIMAGE;
            } else if (imageID == -2 ) {
                qDebug() << "returning dummy image for blacklisted album: " << album;
                return DUMMY_ALBUMIMAGE;
            }
            else {
                qDebug() << "returning database image for album: " << album;
                QString url = "image://imagedbprovider/albumid/" + QString::number(imageID);
                return url;
            }
        }
    }

    return QVariant(QVariant::Invalid);;
}

int PlaylistModel::rowCount(const QModelIndex &parent) const{
    if ( parent.isValid() ) {
        return 0;
    }
    if(mEntries)
        return mEntries->length();
    else
        return 0;
}

QHash<int, QByteArray> PlaylistModel::roleNames() const {
    QHash<int,QByteArray> roles;

    roles[NameRole] = "name";
    roles[pathRole] = "path";
    roles[filenameRole] = "filename";
    roles[titleRole] = "title";
    roles[artistRole] = "artist";
    roles[albumRole] = "album";
    roles[lengthRole] = "length";
    roles[lengthFormatedRole] = "lengthformated";
    roles[tracknoRole] = "tracknr";
    roles[yearRole] = "year";
    roles[playingRole] = "playing";
    roles[sectionRole] = "section";
    roles[sectionImageURLRole] = "sectionImageURL";
    roles[trackmbidRole] = "trackmbid";
    roles[albummbidRole] = "albummbid";
    roles[artistmbidRole] = "artistmbid";

    return roles;
}

void PlaylistModel::setPlaying(quint32 position, bool playing) {
    if(position< (quint32) rowCount()) {
        mEntries->at((int)position)->setPlaying(playing);
        emit dataChanged(createIndex(position,0),createIndex(position,0),QVector<int>(1,playingRole));
    }
}

bool PlaylistModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    if(role == playingRole) {
        mEntries->at(index.row())->setPlaying(value.toBool());
        return true;
    }
    return false;
}

MpdTrack* PlaylistModel::get(int index) {
    if ( rowCount() > index ) {
        MpdTrack *retTrack = mEntries->at(index);
        QQmlEngine::setObjectOwnership(retTrack,QQmlEngine::CppOwnership);
        return retTrack;
    } else {
        return 0;
    }
}

void PlaylistModel::receiveNewTrackList(QList<MpdTrack *>* tracks)
{
    QList<MpdTrack*> *tmpPointer = mEntries;

    beginResetModel();
    mEntries = tracks;
    endResetModel();

    if ( mPlaybackState != MPD_STOP ) {
        setPlaying(mTrackNo,true);
    }
    if ( tmpPointer != 0 ) {
//       qDeleteAll(*tmpPointer);
        QList<MpdTrack*>::Iterator it;
        for ( it = tmpPointer->begin(); it != tmpPointer->end() ; ++it ) {
            (*it)->deleteLater();
        }
       delete(tmpPointer);
    }
}

void PlaylistModel::onPlaybackStateChanged(MpdPlaybackState state)
{
    mPlaybackState = state;
    if ( mPlaybackState == MPD_STOP ) {
        // Unset playing flag for last track played
        setPlaying(mTrackNo,false);
    } else {
        setPlaying(mTrackNo, true);
    }
}

void PlaylistModel::onTrackNoChanged(quint32 trackNo)
{
    // Check boundaries
    if ( trackNo >= (quint32)rowCount() ) {
        mTrackNo = trackNo;
        return;
    }

    // Change track playing flags
    setPlaying(mTrackNo, false);
    mTrackNo = trackNo;
    setPlaying(mTrackNo, true);
}
