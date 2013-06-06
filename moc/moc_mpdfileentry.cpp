/****************************************************************************
** Meta object code from reading C++ file 'mpdfileentry.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/mpdfileentry.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mpdfileentry.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MpdFileEntry[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
      11,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      21,   13, 0x0a095001,
      26,   13, 0x0a095001,
      39,   34, 0x01095001,
      46,   34, 0x01095001,
      58,   13, 0x0a095001,
      64,   13, 0x0a095001,
      70,   13, 0x0a095001,
      75,   13, 0x0a095001,
      82,   13, 0x0a095001,
      93,   89, 0x02095001,
     101,   13, 0x0a095001,

       0        // eod
};

static const char qt_meta_stringdata_MpdFileEntry[] = {
    "MpdFileEntry\0QString\0name\0prepath\0"
    "bool\0isFile\0isDirectory\0title\0album\0"
    "path\0artist\0length\0int\0tracknr\0year\0"
};

void MpdFileEntry::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData MpdFileEntry::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MpdFileEntry::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MpdFileEntry,
      qt_meta_data_MpdFileEntry, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MpdFileEntry::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MpdFileEntry::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MpdFileEntry::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MpdFileEntry))
        return static_cast<void*>(const_cast< MpdFileEntry*>(this));
    return QObject::qt_metacast(_clname);
}

int MpdFileEntry::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = getName(); break;
        case 1: *reinterpret_cast< QString*>(_v) = getPrePath(); break;
        case 2: *reinterpret_cast< bool*>(_v) = isFile(); break;
        case 3: *reinterpret_cast< bool*>(_v) = isDirectory(); break;
        case 4: *reinterpret_cast< QString*>(_v) = getTitle(); break;
        case 5: *reinterpret_cast< QString*>(_v) = getAlbum(); break;
        case 6: *reinterpret_cast< QString*>(_v) = getPath(); break;
        case 7: *reinterpret_cast< QString*>(_v) = getArtist(); break;
        case 8: *reinterpret_cast< QString*>(_v) = getLengthFormatted(); break;
        case 9: *reinterpret_cast< int*>(_v) = getTrackNr(); break;
        case 10: *reinterpret_cast< QString*>(_v) = getYear(); break;
        }
        _id -= 11;
    } else if (_c == QMetaObject::WriteProperty) {
        _id -= 11;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 11;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 11;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
