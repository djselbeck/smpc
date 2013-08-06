# The name of your app
TARGET = jollampc

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

# The .desktop file
desktop.files = jollampc.desktop

# Please do not modify the following line.
include(sailfishapplication/sailfishapplication.pri)

OTHER_FILES = rpm/jollampc.yaml \
    pages/MainPage.qml \
    components/MainGridItem.qml

