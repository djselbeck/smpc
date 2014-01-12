#include "streamplayer.h"

StreamPlayer::StreamPlayer(QObject *parent) :
    QObject(parent)
{
    mPlayer = new QMediaPlayer(this);
}

void StreamPlayer::startPlayback()
{
    mPlayer->play();
}

void StreamPlayer::setURL(QUrl url)
{
    mPlayer->setMedia(url);
}
