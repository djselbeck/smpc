#ifndef QMLIMAGEPROVIDER_H
#define QMLIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QMutex>
#include <QDebug>

#include <localdb/imagedatabase.h>

class QMLImageProvider : public QQuickImageProvider
{
public:
    explicit QMLImageProvider(ImageDatabase *db);
    QPixmap requestPixmap(const QString & id, QSize * size, const QSize & requestedSize);


private:
    ImageDatabase *mDB;
    QMutex mGetMutex;
signals:

public slots:

};

#endif // QMLIMAGEPROVIDER_H
