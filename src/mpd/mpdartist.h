#ifndef MPDARTIST_H
#define MPDARTIST_H

#include <QObject>

class MpdArtist : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString artist READ getName NOTIFY changed )
    Q_PROPERTY(QString sectionprop READ getSection NOTIFY changed )
    Q_PROPERTY(QString mbid READ getMBID NOTIFY changed )
public:
    explicit MpdArtist(QObject *parent = 0);
    MpdArtist(QObject *parent, QString mName);

    MpdArtist(const MpdArtist &copyObject, QObject *parent = 0);

    ~MpdArtist();

    QString getName() const;

    QString getMBID() const;

    QString getSection() const;
    bool operator< (const MpdArtist& other) const;
    bool operator==(MpdArtist & rhs);
    void operator=(MpdArtist &rhs);

    static bool lessThan(const MpdArtist *lhs, const MpdArtist* rhs);


private:
    QString mName;
    QString mMBID;

signals:
    void changed();

public slots:

};

#endif // MPDARTIST_H
