#ifndef SERVERINFO_H
#define SERVERINFO_H

#include "mpdcommon.h"

class ServerInfo
{
public:
    ServerInfo();

    void setVersion(MPD_version_t version);
    void setIdleSupported(bool idle);
    void setListGroupSupported(bool groupSupported);
    void setListFilterSupported(bool filter);
    void setMBIDTagsSupported(bool MBIDSupported);

    MPD_version_t *getVersion();
    bool getIdleSupported();
    bool getListGroupSupported();
    bool getListFilterSupported();
    bool getMBIDTagsSupported();
private:
    /**
     * @brief pVersion Version of the connected MPD server
     */
    MPD_version_t pVersion;

    /**
     * @brief pIdleSupported If IDLE command is supported on server
     */
    bool pIdleSupported;

    /**
     * @brief pListGroupSupportedTrue if grouping is available on queries
     */
    bool pListGroupSupported;

    /**
     * @brief pListFilterSupported True if filtering lists is possible (Artist albums)
     */
    bool pListFilterSupported;

    /**
     * @brief pMBIDTagsSupported True if MusicBrainz IDs are available as tags
     */
    bool pMBIDTagsSupported;
};

#endif // SERVERINFO_H
