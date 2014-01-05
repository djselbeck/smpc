#ifndef MPDARTIST_H
#define MPDARTIST_H

#include <QObject>
#include "mpdalbum.h"
#include "mpdtrack.h"

class MpdTrack;
class MpdAlbum;

class MpdArtist : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString artist READ getName NOTIFY changed )
    Q_PROPERTY(QString sectionprop READ getSection NOTIFY changed )
public:
    explicit MpdArtist(QObject *parent = 0);
    MpdArtist(QObject *parent, QString mName);

    // Copy constructor
    MpdArtist(const MpdArtist &copyObject);

    ~MpdArtist();
//    void addTrack(MpdTrack *track);
//    void addAlbum(MpdAlbum *album);
//    void addAlbums(QList<MpdAlbum*> *albums);
//    quint32 albumCount();
    //MpdAlbum *getAlbum(quint32 i);
    const QString getName();
    QString getSection() { return (mName=="" ? "" :QString(mName.toUpper()[0]));}
    bool operator< (const MpdArtist& other) const { return (mName.compare(other.mName,Qt::CaseInsensitive)<0?1:0); }
    bool operator==(MpdArtist & rhs) {return getName()==rhs.getName();}
    void operator=(MpdArtist &rhs)
    {
       mName = rhs.mName;
    }

    static bool lessThan(const MpdArtist *lhs, const MpdArtist* rhs) {
        return *lhs<*rhs;
    }


private:
    QString mName;
//    QList<MpdAlbum*> *albums;
//    QList<MpdTrack*> *tracks;

signals:
    void changed();

public slots:

};

#endif // MPDARTIST_H
