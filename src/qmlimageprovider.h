#ifndef QMLIMAGEPROVIDER_H
#define QMLIMAGEPROVIDER_H

#include <QQuickImageProvider>

#include "imagedatabase.h"

class QMLImageProvider : public QQuickImageProvider
{
public:
    explicit QMLImageProvider(ImageDatabase *db);
    QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize);


private:
    ImageDatabase *mDB;
signals:

public slots:

};

#endif // QMLIMAGEPROVIDER_H
