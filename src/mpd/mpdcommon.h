#ifndef MPDCOMMON_H
#define MPDCOMMON_H
#include <QtGlobal>

struct MPD_version_t {
    quint32 mpdMajor1;
    quint32 mpdMajor2;
    quint32 mpdMinor;
};

enum MPD_PLAYBACK_STATE {
    MPD_PAUSE,MPD_PLAYING,MPD_STOP
};

#endif // MPDCOMMON_H
