#ifndef COMMONDEBUG_H
#define COMMONDEBUG_H

#include <QObject>
#include <QString>
#define DEBUG

class CommonDebug : public QObject
{
    Q_OBJECT
public:
    explicit CommonDebug(QString);

signals:

public slots:

};

#endif // COMMONDEBUG_H
