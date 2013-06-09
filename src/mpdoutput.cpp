#include "mpdoutput.h"

MPDOutput::MPDOutput(QString name, bool enabled, int id, QObject *parent) :
    QObject(parent)
{
    mName = name;
    mEnabled = enabled;
    mID = id;
}

void MPDOutput::setEnabled(bool en)
{
    mEnabled = en;
    emit activechanged();
}
