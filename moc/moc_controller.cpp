/****************************************************************************
** Meta object code from reading C++ file 'controller.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/controller.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Controller[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      62,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      21,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x05,
      44,   37,   11,   11, 0x05,
      65,   11,   11,   11, 0x05,
      83,   11,   11,   11, 0x05,
     106,  101,   11,   11, 0x05,
     124,   11,   11,   11, 0x05,
     141,   11,   11,   11, 0x05,
     161,   11,   11,   11, 0x05,
     175,   11,   11,   11, 0x05,
     190,   11,   11,   11, 0x05,
     209,   11,   11,   11, 0x05,
     229,   11,   11,   11, 0x05,
     251,   11,   11,   11, 0x05,
     272,   11,   11,   11, 0x05,
     287,   11,   11,   11, 0x05,
     311,   11,   11,   11, 0x05,
     326,   11,   11,   11, 0x05,
     349,   11,   11,   11, 0x05,
     363,   11,   11,   11, 0x05,
     377,   11,   11,   11, 0x05,
     399,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     433,  425,   11,   11, 0x0a,
     465,   11,   11,   11, 0x08,
     490,   11,   11,   11, 0x08,
     506,   11,   11,   11, 0x08,
     530,  523,   11,   11, 0x08,
     565,  559,   11,   11, 0x08,
     588,   11,   11,   11, 0x08,
     613,   11,   11,   11, 0x08,
     639,   11,   11,   11, 0x08,
     649,   11,   11,   11, 0x08,
     661,   11,   11,   11, 0x08,
     673,   37,   11,   11, 0x08,
     705,  701,   11,   11, 0x08,
     725,  701,   11,   11, 0x08,
     746,  701,   11,   11, 0x08,
     768,   11,   11,   11, 0x08,
     788,   11,   11,   11, 0x08,
     816,  811,   11,   11, 0x08,
     852,  811,   11,   11, 0x08,
     889,  811,   11,   11, 0x08,
     927,  811,   11,   11, 0x08,
     962,  811,   11,   11, 0x08,
    1003,  811,   11,   11, 0x08,
    1040,  811,   11,   11, 0x08,
    1088, 1079,   11,   11, 0x08,
    1118, 1109,   11,   11, 0x08,
    1144, 1139,   11,   11, 0x08,
    1157,   11,   11,   11, 0x08,
    1175,   11,   11,   11, 0x08,
    1182,   11,   11,   11, 0x08,
    1203, 1195,   11,   11, 0x08,
    1233, 1227,   11,   11, 0x08,
    1252, 1227,   11,   11, 0x08,
    1272,   11,   11,   11, 0x08,
    1312,  811,   11,   11, 0x08,
    1355,   11,   11,   11, 0x08,
    1377,   11,   11,   11, 0x08,
    1401,   11,   11,   11, 0x08,
    1416,   11,   11,   11, 0x08,
    1433,   11,   11,   11, 0x08,
    1447,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Controller[] = {
    "Controller\0\0text\0sendPopup(QVariant)\0"
    "status\0sendStatus(QVariant)\0"
    "playlistUpdated()\0filesModelReady()\0"
    "path\0getFiles(QString)\0requestConnect()\0"
    "requestDisconnect()\0albumsReady()\0"
    "artistsReady()\0albumTracksReady()\0"
    "artistAlbumsReady()\0savedPlaylistsReady()\0"
    "savedPlaylistReady()\0outputsReady()\0"
    "serverProfilesUpdated()\0setVolume(int)\0"
    "setUpdateInterval(int)\0showWelcome()\0"
    "requestExit()\0searchedTracksReady()\0"
    "addURIToPlaylist(QString)\0old,now\0"
    "focusChanged(QWidget*,QWidget*)\0"
    "requestCurrentPlaylist()\0requestAlbums()\0"
    "requestArtists()\0artist\0"
    "requestArtistAlbums(QString)\0array\0"
    "requestAlbum(QVariant)\0requestFilePage(QString)\0"
    "requestFileModel(QString)\0seek(int)\0"
    "incVolume()\0decVolume()\0"
    "updateStatus(status_struct)\0key\0"
    "mediaKeyHandle(int)\0mediaKeyPressed(int)\0"
    "mediaKeyReleased(int)\0connectedToServer()\0"
    "disconnectedToServer()\0list\0"
    "updateAlbumsModel(QList<QObject*>*)\0"
    "updateArtistsModel(QList<QObject*>*)\0"
    "updatePlaylistModel(QList<QObject*>*)\0"
    "updateFilesModel(QList<QObject*>*)\0"
    "updateAlbumTracksModel(QList<QObject*>*)\0"
    "updateOutputsModel(QList<QObject*>*)\0"
    "updateSearchedTracks(QList<QObject*>*)\0"
    "hostname\0setHostname(QString)\0password\0"
    "setPassword(QString)\0port\0setPort(int)\0"
    "connectToServer()\0quit()\0newProfile()\0"
    "profile\0changeProfile(QVariant)\0index\0"
    "deleteProfile(int)\0connectProfile(int)\0"
    "updateSavedPlaylistsModel(QStringList*)\0"
    "updateSavedPlaylistModel(QList<QObject*>*)\0"
    "applicationActivate()\0applicationDeactivate()\0"
    "fileStackPop()\0cleanFileStack()\0"
    "exitRequest()\0addlastsearchtoplaylist()\0"
};

void Controller::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Controller *_t = static_cast<Controller *>(_o);
        switch (_id) {
        case 0: _t->sendPopup((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 1: _t->sendStatus((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 2: _t->playlistUpdated(); break;
        case 3: _t->filesModelReady(); break;
        case 4: _t->getFiles((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->requestConnect(); break;
        case 6: _t->requestDisconnect(); break;
        case 7: _t->albumsReady(); break;
        case 8: _t->artistsReady(); break;
        case 9: _t->albumTracksReady(); break;
        case 10: _t->artistAlbumsReady(); break;
        case 11: _t->savedPlaylistsReady(); break;
        case 12: _t->savedPlaylistReady(); break;
        case 13: _t->outputsReady(); break;
        case 14: _t->serverProfilesUpdated(); break;
        case 15: _t->setVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->setUpdateInterval((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->showWelcome(); break;
        case 18: _t->requestExit(); break;
        case 19: _t->searchedTracksReady(); break;
        case 20: _t->addURIToPlaylist((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->focusChanged((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 22: _t->requestCurrentPlaylist(); break;
        case 23: _t->requestAlbums(); break;
        case 24: _t->requestArtists(); break;
        case 25: _t->requestArtistAlbums((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->requestAlbum((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 27: _t->requestFilePage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->requestFileModel((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 29: _t->seek((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->incVolume(); break;
        case 31: _t->decVolume(); break;
        case 32: _t->updateStatus((*reinterpret_cast< status_struct(*)>(_a[1]))); break;
        case 33: _t->mediaKeyHandle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->mediaKeyPressed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->mediaKeyReleased((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->connectedToServer(); break;
        case 37: _t->disconnectedToServer(); break;
        case 38: _t->updateAlbumsModel((*reinterpret_cast< QList<QObject*>*(*)>(_a[1]))); break;
        case 39: _t->updateArtistsModel((*reinterpret_cast< QList<QObject*>*(*)>(_a[1]))); break;
        case 40: _t->updatePlaylistModel((*reinterpret_cast< QList<QObject*>*(*)>(_a[1]))); break;
        case 41: _t->updateFilesModel((*reinterpret_cast< QList<QObject*>*(*)>(_a[1]))); break;
        case 42: _t->updateAlbumTracksModel((*reinterpret_cast< QList<QObject*>*(*)>(_a[1]))); break;
        case 43: _t->updateOutputsModel((*reinterpret_cast< QList<QObject*>*(*)>(_a[1]))); break;
        case 44: _t->updateSearchedTracks((*reinterpret_cast< QList<QObject*>*(*)>(_a[1]))); break;
        case 45: _t->setHostname((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 46: _t->setPassword((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 47: _t->setPort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 48: _t->connectToServer(); break;
        case 49: _t->quit(); break;
        case 50: _t->newProfile(); break;
        case 51: _t->changeProfile((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 52: _t->deleteProfile((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 53: _t->connectProfile((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 54: _t->updateSavedPlaylistsModel((*reinterpret_cast< QStringList*(*)>(_a[1]))); break;
        case 55: _t->updateSavedPlaylistModel((*reinterpret_cast< QList<QObject*>*(*)>(_a[1]))); break;
        case 56: _t->applicationActivate(); break;
        case 57: _t->applicationDeactivate(); break;
        case 58: _t->fileStackPop(); break;
        case 59: _t->cleanFileStack(); break;
        case 60: _t->exitRequest(); break;
        case 61: _t->addlastsearchtoplaylist(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Controller::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Controller::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Controller,
      qt_meta_data_Controller, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Controller::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Controller::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Controller::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Controller))
        return static_cast<void*>(const_cast< Controller*>(this));
    return QObject::qt_metacast(_clname);
}

int Controller::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 62)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 62;
    }
    return _id;
}

// SIGNAL 0
void Controller::sendPopup(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Controller::sendStatus(QVariant _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Controller::playlistUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Controller::filesModelReady()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Controller::getFiles(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Controller::requestConnect()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void Controller::requestDisconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void Controller::albumsReady()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void Controller::artistsReady()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void Controller::albumTracksReady()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}

// SIGNAL 10
void Controller::artistAlbumsReady()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void Controller::savedPlaylistsReady()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void Controller::savedPlaylistReady()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void Controller::outputsReady()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void Controller::serverProfilesUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 14, 0);
}

// SIGNAL 15
void Controller::setVolume(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void Controller::setUpdateInterval(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void Controller::showWelcome()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}

// SIGNAL 18
void Controller::requestExit()
{
    QMetaObject::activate(this, &staticMetaObject, 18, 0);
}

// SIGNAL 19
void Controller::searchedTracksReady()
{
    QMetaObject::activate(this, &staticMetaObject, 19, 0);
}

// SIGNAL 20
void Controller::addURIToPlaylist(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}
QT_END_MOC_NAMESPACE
