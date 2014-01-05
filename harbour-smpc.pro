# The name of your app
TARGET = harbour-smpc

QT += network gui sql

CONFIG += sailfishapp


# C++ sources
SOURCES += main.cpp \
            src/networkaccess.cpp \
            src/controller.cpp \
            src/mpdtrack.cpp \
            src/mpdfileentry.cpp \
            src/mpdartist.cpp \
            src/mpdalbum.cpp \
            src/qthreadex.cpp \
            src/serverprofile.cpp \
            src/artistmodel.cpp \
            src/mpdoutput.cpp \
            src/albummodel.cpp \
    src/filemodel.cpp \
    src/playlistmodel.cpp \
    src/imagedatabase.cpp \
    src/imagedownloader.cpp \
    src/lastfmalbumprovider.cpp \
    src/albuminformation.cpp \
    src/databasefilljob.cpp \
    src/qmlimageprovider.cpp \
    src/lastfmartistprovider.cpp \
    src/artistinformation.cpp


# C++ headers
HEADERS += \
    src/controller.h \
    src/networkaccess.h \
    src/mpdtrack.h \
    src/mpdfileentry.h \
    src/mpdartist.h \
    src/mpdalbum.h \
    src/common.h \
    src/qthreadex.h \
    src/serverprofile.h \
    src/artistmodel.h \
    src/albummodel.h \
    src/mpdoutput.h \
    src/filemodel.h \
    src/playlistmodel.h \
    src/imagedatabase.h \
    src/imagedownloader.h \
    src/lastfmalbumprovider.h \
    src/albuminformation.h \
    src/databasefilljob.h \
    src/qmlimageprovider.h \
    src/lastfmartistprovider.h \
    src/artistinformation.h \
    src/databasestatistic.h

# QML files and folders
#qml.files = *.qml pages cover main.qml components


# Please do not modify the following line.
#include(sailfishapplication/sailfishapplication.pri)


OTHER_FILES = rpm/harbour-smpc.yaml \
              qml/main.qml \
    qml/pages/SongPage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/ServerListPage.qml \
    qml/pages/ServerEditPage.qml \
    qml/pages/SearchPage.qml \
    qml/pages/SavedPlaylistsPage.qml \
    qml/pages/PlaylistTracksPage.qml \
    qml/pages/OutputsPage.qml \
    qml/pages/MainPage.qml \
    qml/pages/FileBrowserPage.qml \
    qml/pages/CurrentSong.qml \
    qml/pages/CurrentPlaylistPage.qml \
    qml/pages/ConnectServerPage.qml \
    qml/pages/ArtistListPage.qml \
    qml/pages/AlbumTracksPage.qml \
    qml/pages/AlbumListPage.qml \
    qml/pages/AboutPage.qml \
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
    qml/components/ArtistDelegate.qml \
    qml/components/AlbumDelegate.qml \
    qml/cover/CoverPage.qml \
    icon-m-stop.png \
    qml/components/ControlPanel.qml \
    qml/pages/DatabaseSettings.qml \
    qml/pages/ArtistInformationPage.qml


RESOURCES += \
    translations.qrc \
    miscresources.qrc

DEFINES += QT_USE_FAST_CONCATENATION QT_USE_FAST_OPERATOR_PLUS

TRANSLATIONS += $${TARGET}_de.ts

evil_hack_to_fool_lupdate {
SOURCES += \
              qml/main.qml \
    qml/pages/SongPage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/ServerListPage.qml \
    qml/pages/ServerEditPage.qml \
    qml/pages/SearchPage.qml \
    qml/pages/SavedPlaylistsPage.qml \
    qml/pages/PlaylistTracksPage.qml \
    qml/pages/OutputsPage.qml \
    qml/pages/MainPage.qml \
    qml/pages/FileBrowserPage.qml \
    qml/pages/CurrentSong.qml \
    qml/pages/CurrentPlaylistPage.qml \
    qml/pages/ConnectServerPage.qml \
    qml/pages/ArtistListPage.qml \
    qml/pages/AlbumTracksPage.qml \
    qml/pages/AlbumListPage.qml \
    qml/pages/AboutPage.qml \
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
    qml/components/ArtistDelegate.qml \
    qml/components/AlbumDelegate.qml \
    qml/cover/CoverPage.qml \
    qml/components/ControlPanel.qml
}
