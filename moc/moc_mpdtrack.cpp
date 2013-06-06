/****************************************************************************
** Meta object code from reading C++ file 'mpdtrack.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/mpdtrack.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mpdtrack.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MpdTrack[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
      10,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

 // properties: name, type, flags
      35,   27, 0x0a095001,
      41,   27, 0x0a095001,
      45,   27, 0x0a095001,
      64,   60, 0x02095001,
      71,   27, 0x0a095001,
      78,   27, 0x0a095001,
      89,   84, 0x01495001,
      97,   60, 0x02095001,
     105,   27, 0x0a095001,
     110,   27, 0x0a095001,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,
       0,

       0        // eod
};

static const char qt_meta_stringdata_MpdTrack[] = {
    "MpdTrack\0\0playingchanged()\0QString\0"
    "title\0uri\0lengthformated\0int\0length\0"
    "artist\0album\0bool\0playing\0tracknr\0"
    "year\0filename\0"
};

void MpdTrack::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MpdTrack *_t = static_cast<MpdTrack *>(_o);
        switch (_id) {
        case 0: _t->playingchanged(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MpdTrack::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MpdTrack::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MpdTrack,
      qt_meta_data_MpdTrack, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MpdTrack::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MpdTrack::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MpdTrack::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MpdTrack))
        return static_cast<void*>(const_cast< MpdTrack*>(this));
    return QObject::qt_metacast(_clname);
}

int MpdTrack::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = getTitle(); break;
        case 1: *reinterpret_cast< QString*>(_v) = getFileUri(); break;
        case 2: *reinterpret_cast< QString*>(_v) = getLengthFormated(); break;
        case 3: *reinterpret_cast< int*>(_v) = getLength(); break;
        case 4: *reinterpret_cast< QString*>(_v) = getArtist(); break;
        case 5: *reinterpret_cast< QString*>(_v) = getAlbum(); break;
        case 6: *reinterpret_cast< bool*>(_v) = getPlaying(); break;
        case 7: *reinterpret_cast< int*>(_v) = getTrackNr(); break;
        case 8: *reinterpret_cast< QString*>(_v) = getYear(); break;
        case 9: *reinterpret_cast< QString*>(_v) = getFileName(); break;
        }
        _id -= 10;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 10;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 10;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 10;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void MpdTrack::playingchanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
