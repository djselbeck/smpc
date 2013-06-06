/****************************************************************************
** Meta object code from reading C++ file 'serverprofile.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/serverprofile.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serverprofile.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ServerProfile[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       5,   19, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // properties: name, type, flags
      38,   30, 0x0a495103,
      47,   30, 0x0a495103,
      56,   30, 0x0a495103,
      65,   61, 0x02495103,
      75,   70, 0x01095103,

 // properties: notify_signal_id
       0,
       0,
       0,
       0,
       0,

       0        // eod
};

static const char qt_meta_stringdata_ServerProfile[] = {
    "ServerProfile\0\0valueChanged()\0QString\0"
    "hostname\0password\0name\0int\0port\0bool\0"
    "autoconnect\0"
};

void ServerProfile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ServerProfile *_t = static_cast<ServerProfile *>(_o);
        switch (_id) {
        case 0: _t->valueChanged(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ServerProfile::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ServerProfile::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ServerProfile,
      qt_meta_data_ServerProfile, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ServerProfile::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ServerProfile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ServerProfile::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ServerProfile))
        return static_cast<void*>(const_cast< ServerProfile*>(this));
    return QObject::qt_metacast(_clname);
}

int ServerProfile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
        case 0: *reinterpret_cast< QString*>(_v) = getHostname(); break;
        case 1: *reinterpret_cast< QString*>(_v) = getPassword(); break;
        case 2: *reinterpret_cast< QString*>(_v) = getName(); break;
        case 3: *reinterpret_cast< int*>(_v) = getPort(); break;
        case 4: *reinterpret_cast< bool*>(_v) = getAutoconnect(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setHostname(*reinterpret_cast< QString*>(_v)); break;
        case 1: setPassword(*reinterpret_cast< QString*>(_v)); break;
        case 2: setName(*reinterpret_cast< QString*>(_v)); break;
        case 3: setPort(*reinterpret_cast< int*>(_v)); break;
        case 4: setAutoconnect(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ServerProfile::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
