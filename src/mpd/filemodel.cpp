#include "filemodel.h"

FileModel::FileModel(QObject *parent) :
    QAbstractListModel(parent)
{
}


FileModel::FileModel(QList<MpdFileEntry *> *list,
                     ImageDatabase *db,
                     QObject *parent) : QAbstractListModel(parent)
{
    mEntries = list;
    mDB = db;
}

FileModel::~FileModel()
{
    for(int i=0;i<mEntries->length();i++)
    {
        delete(mEntries->at(i));
    }
    delete(mEntries);
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if(role==NameRole)
    {
        return mEntries->at(index.row())->getName();
    }
    else if(role==SectionRole)
        return mEntries->at(index.row())->getSection();
    else if(role==isDirectoryRole)
        return mEntries->at(index.row())->isDirectory();
    else if(role==isFileRole)
        return mEntries->at(index.row())->isFile();
    else if(role==isPlaylistRole)
        return mEntries->at(index.row())->isPlaylist();
    else if(role==pathRole)
        return mEntries->at(index.row())->getPath();
    else if(role==prePathRole)
        return mEntries->at(index.row())->getPrePath();
    else if(role==titleRole)
        return mEntries->at(index.row())->getTitle();
    else if(role==artistRole)
        return mEntries->at(index.row())->getArtist();
    else if(role==albumRole)
        return mEntries->at(index.row())->getAlbum();
    else if(role==lengthRole)
        return mEntries->at(index.row())->getLengthFormatted();
    else if(role==tracknoRole)
        return mEntries->at(index.row())->getTrackNr();
    else if(role==yearRole)
        return mEntries->at(index.row())->getYear();
    else if(role==imageURLRole) {
        MpdFileEntry *mpdFile = mEntries->at(index.row());
        if ( mpdFile->isFile() ) {
            QString album = mpdFile->getAlbum();
            QString artist = mpdFile->getArtist();
            if ( artist != "" ) {
                int imageID = mDB->imageIDFromAlbumArtist(album,artist);
                // No image found return dummy url
                if ( imageID <= -1 ) {
                    // Start image retrieval
                    qDebug() << "returning dummy image for album: " << album;
                    // Return dummy for the time being
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
                if ( imageID <= -1 ) {
                    // Start image retrieval
                    qDebug() << "returning dummy image for album: " << album;
                    // Return dummy for the time being
                    return DUMMY_ALBUMIMAGE;
                }
                else {
                    qDebug() << "returning database image for album: " << album;
                    QString url = "image://imagedbprovider/albumid/" + QString::number(imageID);
                    return url;
                }
            }
        }
    }
    return 0;
}

int FileModel::rowCount(const QModelIndex &parent) const
{
    if ( mEntries )
        return mEntries->length();
    return 0;
}

QHash<int, QByteArray> FileModel::roleNames() const {
    QHash<int,QByteArray> roles;

    roles[NameRole] = "name";
    roles[SectionRole] = "sectionprop";
    roles[isDirectoryRole] = "isDirectory";
    roles[isFileRole] = "isFile";
    roles[isPlaylistRole] = "isPlaylist";
    roles[prePathRole] = "prepath";
    roles[pathRole] = "path";
    roles[titleRole] = "title";
    roles[artistRole] = "artist";
    roles[albumRole] = "album";
    roles[lengthRole] = "length";
    roles[tracknoRole] = "tracknr";
    roles[yearRole] = "year";
    roles[imageURLRole] = "imageURL";

    return roles;
}

MpdFileEntry *FileModel::get(int index)
{
    MpdFileEntry *fileEntry = mEntries->at(index);
    QQmlEngine::setObjectOwnership(fileEntry,QQmlEngine::CppOwnership);
    return fileEntry;
}
