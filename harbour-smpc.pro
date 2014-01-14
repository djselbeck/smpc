# The name of your app
TARGET = harbour-smpc

QT += network gui sql multimedia

CONFIG += sailfishapp


# C++ sources
SOURCES += main.cpp \
    src/localdb/qmlimageprovider.cpp \
    src/localdb/lastfmartistprovider.cpp \
    src/localdb/lastfmalbumprovider.cpp \
    src/localdb/imagedownloader.cpp \
    src/localdb/imagedatabase.cpp \
    src/localdb/databasefilljob.cpp \
    src/localdb/artistinformation.cpp \
    src/localdb/albuminformation.cpp \
    src/mpd/serverprofile.cpp \
    src/mpd/playlistmodel.cpp \
    src/mpd/networkaccess.cpp \
    src/mpd/mpdtrack.cpp \
    src/mpd/mpdoutput.cpp \
    src/mpd/mpdfileentry.cpp \
    src/mpd/mpdartist.cpp \
    src/mpd/mpdalbum.cpp \
    src/mpd/filemodel.cpp \
    src/mpd/artistmodel.cpp \
    src/mpd/albummodel.cpp \
    src/controller.cpp \
    src/streamplayer.cpp


# C++ headers
HEADERS += \
    src/controller.h \
    src/localdb/qmlimageprovider.h \
    src/localdb/lastfmartistprovider.h \
    src/localdb/lastfmalbumprovider.h \
    src/localdb/imagedownloader.h \
    src/localdb/imagedatabase.h \
    src/localdb/databasestatistic.h \
    src/localdb/databasefilljob.h \
    src/localdb/artistinformation.h \
    src/localdb/albuminformation.h \
    src/mpd/serverprofile.h \
    src/mpd/playlistmodel.h \
    src/mpd/networkaccess.h \
    src/mpd/mpdtrack.h \
    src/mpd/mpdoutput.h \
    src/mpd/mpdfileentry.h \
    src/mpd/mpdartist.h \
    src/mpd/mpdalbum.h \
    src/mpd/filemodel.h \
    src/mpd/artistmodel.h \
    src/mpd/albummodel.h \
    src/common.h \
    src/streamplayer.h

OTHER_FILES = rpm/harbour-smpc.yaml \
    icon-m-stop.png \ 
    qml/components/qmldir \
    qml/pages/database/SongPage.qml \
    qml/pages/database/SearchPage.qml \
    qml/pages/database/SavedPlaylistsPage.qml \
    qml/pages/database/PlaylistTracksPage.qml \
    qml/pages/database/FileBrowserPage.qml \
    qml/pages/database/CurrentSong.qml \
    qml/pages/database/CurrentPlaylistPage.qml \
    qml/pages/database/ArtistListPage.qml \
    qml/pages/database/ArtistInformationPage.qml \
    qml/pages/database/ArtistInfoPage.qml \
    qml/pages/database/AlbumTracksPage.qml \
    qml/pages/database/AlbumListPage.qml \
    qml/pages/database/AlbumInfoPage.qml \
    qml/pages/settings/SettingsPage.qml \
    qml/pages/settings/ServerListPage.qml \
    qml/pages/settings/ServerEditPage.qml \
    qml/pages/settings/OutputsPage.qml \
    qml/pages/settings/DatabaseSettings.qml \
    qml/pages/settings/ConnectServerPage.qml \
    qml/pages/settings/AboutPage.qml \
    qml/main.qml \
    qml/pages/MainPage.qml \
    qml/components/SpeedScroller.js \
    qml/components/SectionScroller.js \
    qml/components/ToggleImage.qml \
    qml/components/SpeedScroller.qml \
    qml/components/SongDialog.qml \
    qml/components/SectionScroller.qml \
    qml/components/ScrollLabel.qml \
    qml/components/MainGridItem.qml \
    qml/components/Heading.qml \
    qml/components/FileDelegate.qml \
    qml/components/ControlPanel.qml \
    qml/components/ArtistDelegate.qml \
    qml/components/AlbumDelegate.qml \
    qml/cover/CoverPage.qml \
    qml/components/AlbumShowDelegate.qml \
    qml/components/ArtistShowDelegate.qml



RESOURCES += \
    translations.qrc \
    miscresources.qrc


DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS QT_NO_DEBUG_OUTPUT
#DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

TRANSLATIONS += $${TARGET}_de.ts

INCLUDEPATH += src

evil_hack_to_fool_lupdate {
SOURCES += \
    qml/pages/database/SongPage.qml \
    qml/pages/database/SearchPage.qml \
    qml/pages/database/SavedPlaylistsPage.qml \
    qml/pages/database/PlaylistTracksPage.qml \
    qml/pages/database/FileBrowserPage.qml \
    qml/pages/database/CurrentSong.qml \
    qml/pages/database/CurrentPlaylistPage.qml \
    qml/pages/database/ArtistListPage.qml \
    qml/pages/database/ArtistInformationPage.qml \
    qml/pages/database/ArtistInfoPage.qml \
    qml/pages/database/AlbumTracksPage.qml \
    qml/pages/database/AlbumListPage.qml \
    qml/pages/database/AlbumInfoPage.qml \
    qml/pages/settings/SettingsPage.qml \
    qml/pages/settings/ServerListPage.qml \
    qml/pages/settings/ServerEditPage.qml \
    qml/pages/settings/OutputsPage.qml \
    qml/pages/settings/DatabaseSettings.qml \
    qml/pages/settings/ConnectServerPage.qml \
    qml/pages/settings/AboutPage.qml \
    qml/main.qml \
    qml/pages/MainPage.qml \
    qml/components/SpeedScroller.js \
    qml/components/SectionScroller.js \
    qml/components/ToggleImage.qml \
    qml/components/SpeedScroller.qml \
    qml/components/SongDialog.qml \
    qml/components/SectionScroller.qml \
    qml/components/ScrollLabel.qml \
    qml/components/MainGridItem.qml \
    qml/components/Heading.qml \
    qml/components/FileDelegate.qml \
    qml/components/ControlPanel.qml \
    qml/components/ArtistDelegate.qml \
    qml/components/AlbumDelegate.qml
}

