#include <mpd/mpdartist.h>

MpdArtist::MpdArtist(QObject *parent) :
    QObject(parent)
{
}

MpdArtist::MpdArtist(QObject *parent, QString name) : QObject(parent)
{
    this->mName = name;
}

MpdArtist::MpdArtist(const MpdArtist &copyObject, QObject *parent) : QObject(parent) {
    this->mName = copyObject.mName;
}


const QString MpdArtist::getName()
{
   return this->mName;
}

const QString MpdArtist::getMBID() {
    return mMBID;
}


MpdArtist::~MpdArtist()
{
}

QString MpdArtist::getSection()
{
    return (mName=="" ? "" :QString(mName.toUpper()[0]));
}
bool MpdArtist::operator< (const MpdArtist& other) const
{
    return (mName.compare(other.mName,Qt::CaseInsensitive)<0?1:0);
}
bool MpdArtist::operator==(MpdArtist & rhs)
{
    return getName()==rhs.getName();
}
void MpdArtist::operator=(MpdArtist &rhs)
{
   mName = rhs.mName;
}

bool MpdArtist::lessThan(const MpdArtist *lhs, const MpdArtist* rhs) {
    return *lhs<*rhs;
}
