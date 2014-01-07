#ifndef ARTISTMODEL_H
#define ARTISTMODEL_H
#include <QAbstractListModel>
#include <QQmlEngine>

#include <mpd/mpdartist.h>
#include <localdb/imagedatabase.h>

#define DUMMY_ARTISTIMAGE "image://theme/icon-l-music"


class ArtistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
public:
    explicit ArtistModel(QObject *parent = 0);
    ArtistModel(QList<MpdArtist*>* list, ImageDatabase *DB, QObject *parent = 0);
    ~ArtistModel();

    Q_INVOKABLE MpdArtist* get(int index);
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const;

    enum EntryRoles {
        NameRole = Qt::UserRole + 1,
        SectionRole,
        ImageURLRole
    };

private:
    QList<MpdArtist*>* mEntries;

    // Imagedatabase for image retrieval
    ImageDatabase *mImgDB;

signals:

public slots:

};

#endif // ARTISTMODEL_H
