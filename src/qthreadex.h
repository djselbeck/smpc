#ifndef QTHREADEX_H
#define QTHREADEX_H

#include <QThread>

class QThreadEx : public QThread
{
    Q_OBJECT
public:
    explicit QThreadEx(QObject *parent = 0);
    void run();

signals:

public slots:

};

#endif // QTHREADEX_H
