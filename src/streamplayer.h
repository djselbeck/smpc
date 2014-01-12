#ifndef STREAMPLAYER_H
#define STREAMPLAYER_H

#include <QObject>
#include <QMediaPlayer>

class StreamPlayer : public QObject
{
    Q_OBJECT
public:
    explicit StreamPlayer(QObject *parent = 0);


private:
    QString mPlaybackUrl;
    QMediaPlayer *mPlayer;

signals:

public slots:
    void setURL(QUrl url);
    void startPlayback();

};

#endif // STREAMPLAYER_H
