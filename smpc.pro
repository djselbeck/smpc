# The name of your app
TARGET = smpc

QT += network gui

# C++ sources
SOURCES += main.cpp \
            src/networkaccess.cpp \
            src/controller.cpp \
            src/mpdtrack.cpp \
            src/mpdfileentry.cpp \
            src/mpdartist.cpp \
            src/mpdalbum.cpp \
            src/commondebug.cpp \
            src/qthreadex.cpp \
            src/serverprofile.cpp \
            src/artistmodel.cpp \
            src/mpdoutput.cpp \
            src/albummodel.cpp


# C++ headers
HEADERS += \
    src/controller.h \
    src/networkaccess.h \
    src/mpdtrack.h \
    src/mpdfileentry.h \
    src/mpdartist.h \
    src/mpdalbum.h \
    src/commondebug.h \
    src/common.h \
    src/qthreadex.h \
    src/serverprofile.h \
    src/artistmodel.h \
    src/albummodel.h \
    src/mpdoutput.h

# QML files and folders
qml.files = *.qml pages cover main.qml components


TRANSLATIONS += $${TARGET}_de.ts


# The .desktop file
desktop.files = $${TARGET}.desktop
icon.files = $${TARGET}.png
icon.path = /usr/share/icons/

INSTALLS += icon


# Please do not modify the following line.
include(sailfishapplication/sailfishapplication.pri)


OTHER_FILES = rpm/smpc.yaml \
    smpc.png

evil_hack_to_fool_lupdate {
SOURCES += \
  pages/AboutPage.qml \
pages/AlbumListPage.qml \
pages/AlbumTracksPage.qml \
pages/ArtistListPage.qml \
pages/ConnectServerPage.qml \
pages/CurrentPlaylistPage.qml \
pages/CurrentSong.qml \
pages/FileBrowserPage.qml \
pages/MainPage.qml \
pages/OutputsPage.qml \
pages/PlaylistTracksPage.qml \
pages/SavedPlaylistsPage.qml \
pages/SearchPage.qml \
pages/ServerEditPage.qml \
pages/ServerListPage.qml \
pages/SettingsPage.qml \
pages/SongPage.qml \
}

RESOURCES += \
    translations.qrc

