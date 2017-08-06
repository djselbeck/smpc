#ifndef MPDCOMMON_H
#define MPDCOMMON_H

struct MPD_version_t {
    quint32 mpdMajor1;
    quint32 mpdMajor2;
    quint32 mpdMinor;
};

struct MPD_server_info_t {
    /* Version of the server */
    MPD_version_t version;
    /* Capabilities of the connected server */
    bool mpd_cmd_list_group_capabilites;
    bool mpd_cmd_list_filter_criteria;
    bool mpd_cmd_idle;

    bool mpd_tags_musicbrainz;
};

enum MPD_PLAYBACK_STATE {
    MPD_PAUSE,MPD_PLAYING,MPD_STOP
};

#endif // MPDCOMMON_H
