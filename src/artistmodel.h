#ifndef ARTISTMODEL_H
#define ARTISTMODEL_H
#include "mpdartist.h"
#include <QAbstractListModel>
#include <QQmlEngine>

class ArtistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount)
public:
    explicit ArtistModel(QObject *parent = 0);
    ~ArtistModel()
    {
                   for(int i=0;i<m_entries->length();i++)
                   {
                       delete(m_entries->at(i));
                   }
                   delete(m_entries);
                  }

        ArtistModel(QList<MpdArtist*>* list,QObject *parent = 0);
    enum EntryRoles {
        NameRole = Qt::UserRole + 1,
        SectionRole = Qt::UserRole + 2
    };

    Q_INVOKABLE MpdArtist* get(int index) {
        MpdArtist *retArtist = m_entries->at(index);
        QQmlEngine::setObjectOwnership(retArtist,QQmlEngine::CppOwnership);
        return retArtist; }
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const;


private:
    QList<MpdArtist*>* m_entries;

signals:

public slots:

};

#endif // ARTISTMODEL_H
