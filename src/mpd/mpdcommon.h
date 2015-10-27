#ifndef MPDCOMMON_H
#define MPDCOMMON_H

struct MpdVersion {
    quint32 mpdMajor1;
    quint32 mpdMajor2;
    quint32 mpdMinor;
};

struct MpdServerInfo {
    /* Version of the server */
    MpdVersion version;
    /* Capabilities of the connected server */
    bool mpd_cmd_list_group_capabilites;
    bool mpd_cmd_list_filter_criteria;
};

enum MpdPlaybackState {
    MPD_PAUSE,MPD_PLAYING,MPD_STOP
};

#endif // MPDCOMMON_H
