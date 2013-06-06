#include "commondebug.h"
//#define DEBUG
CommonDebug::CommonDebug(QString out)
{
#ifdef DEBUG
    qDebug(out.toAscii());
#endif
}

