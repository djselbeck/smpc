#include "playlistmodel.h"

PlaylistModel::PlaylistModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

PlaylistModel::PlaylistModel(QList<MpdTrack *> *list, ImageDatabase *db, QObject *parent) : QAbstractListModel(parent)
{
    mEntries = list;
    mDB = db;
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

    return 0;
}

int PlaylistModel::rowCount(const QModelIndex &parent) const{
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

    return roles;
}

void PlaylistModel::setPlaying(int position, bool playing) {
    if(position<mEntries->length()) {
        mEntries->at(position)->setPlaying(playing);
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
    if ( mEntries && (mEntries->size() > index) ) {
        MpdTrack *retTrack = mEntries->at(index);
        QQmlEngine::setObjectOwnership(retTrack,QQmlEngine::CppOwnership);
        return retTrack;
    } else {
        return 0;
    }
}
