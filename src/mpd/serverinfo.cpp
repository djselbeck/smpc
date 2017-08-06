#include "serverinfo.h"

ServerInfo::ServerInfo()
{

}

void ServerInfo::setVersion(MPD_version_t version) {
    pVersion = version;
}

void ServerInfo::setIdleSupported(bool idle) {
    pIdleSupported = idle;
}

void ServerInfo::setListFilterSupported(bool filter) {
    pListFilterSupported = filter;
}

void ServerInfo::setListGroupSupported(bool groupSupported) {
    pListGroupSupported = groupSupported;
}
void ServerInfo::setMBIDTagsSupported(bool MBIDSupported) {
    pMBIDTagsSupported = MBIDSupported;
}

MPD_version_t *ServerInfo::getVersion() {
    return &pVersion;
}
bool ServerInfo::getIdleSupported() {
    return pIdleSupported;
}

bool ServerInfo::getListFilterSupported() {
    return pListFilterSupported;
}

bool ServerInfo::getListGroupSupported() {
    return pListGroupSupported;
}
bool ServerInfo::getMBIDTagsSupported() {
    return pMBIDTagsSupported;
}
