#include "qthreadex.h"

QThreadEx::QThreadEx(QObject *parent) :
    QThread(parent)
{

}

void QThreadEx::run(){
    exec();
}

