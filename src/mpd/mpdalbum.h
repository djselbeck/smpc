#ifndef MPDALBUM_H
#define MPDALBUM_H

#include <QObject>

class MpdAlbum : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ getTitle NOTIFY changed )
    Q_PROPERTY(QString sectionprop READ getSection NOTIFY changed )
    Q_PROPERTY(QString mbid READ getMBID NOTIFY changed )
    Q_PROPERTY(QString artist READ getArtist NOTIFY changed )
public:
    explicit MpdAlbum(QObject *parent = 0);
    MpdAlbum(QObject *parent,QString mTitle,QString artist="", QString mbid="");


    MpdAlbum(const MpdAlbum &copyObject, QObject *parent = 0);

    QString getTitle() const;
    QString getSection() const;
    QString getArtist() const;
    QString getMBID() const;

    bool operator< (const MpdAlbum& rhs) const;
    bool operator==(MpdAlbum & rhs) const ;
    void operator=(MpdAlbum &rhs);

    static bool lessThan(const MpdAlbum *lhs, const MpdAlbum* rhs);
private:
    QString mTitle;
    QString mArtist;
    QString mMBID;

signals:
    // Dummy signal, so qml shuts up
    void changed();

public slots:

};

#endif // MPDALBUM_H
