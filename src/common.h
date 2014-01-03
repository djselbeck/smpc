#ifndef COMMON_H
#define COMMON_H

#define VERSION "1.0.1"

// ONLY FOR USE WITH JOLLAMPC
#define LASTFMAPIKEY "1c728df8f626849518971eaae29e01a1"

#include <QString>

struct serverprofile { QString profilename; QString hostname; QString password; quint16 port;bool defaultprofile;};


#endif // COMMON_H

