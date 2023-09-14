/****************************************************************************
** Meta object code from reading C++ file 'server.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../MessangerServer/server.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyServer_t {
    const uint offsetsAndSize[30];
    char stringdata0[168];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MyServer_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MyServer_t qt_meta_stringdata_MyServer = {
    {
QT_MOC_LITERAL(0, 8), // "MyServer"
QT_MOC_LITERAL(9, 9), // "s_started"
QT_MOC_LITERAL(19, 0), // ""
QT_MOC_LITERAL(20, 9), // "s_connect"
QT_MOC_LITERAL(30, 12), // "QHostAddress"
QT_MOC_LITERAL(43, 6), // "s_read"
QT_MOC_LITERAL(50, 11), // "NetworkData"
QT_MOC_LITERAL(62, 12), // "s_disconnect"
QT_MOC_LITERAL(75, 17), // "slotNewConnection"
QT_MOC_LITERAL(93, 14), // "slotServerRead"
QT_MOC_LITERAL(108, 9), // "slotWrite"
QT_MOC_LITERAL(118, 8), // "aAddress"
QT_MOC_LITERAL(127, 6), // "aReply"
QT_MOC_LITERAL(134, 10), // "slotsWrite"
QT_MOC_LITERAL(145, 22) // "slotClientDisconnected"

    },
    "MyServer\0s_started\0\0s_connect\0"
    "QHostAddress\0s_read\0NetworkData\0"
    "s_disconnect\0slotNewConnection\0"
    "slotServerRead\0slotWrite\0aAddress\0"
    "aReply\0slotsWrite\0slotClientDisconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyServer[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   68,    2, 0x06,    1 /* Public */,
       3,    1,   71,    2, 0x06,    3 /* Public */,
       5,    2,   74,    2, 0x06,    5 /* Public */,
       7,    1,   79,    2, 0x06,    8 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   82,    2, 0x0a,   10 /* Public */,
       9,    0,   83,    2, 0x0a,   11 /* Public */,
      10,    2,   84,    2, 0x0a,   12 /* Public */,
      13,    1,   89,    2, 0x0a,   15 /* Public */,
      14,    0,   92,    2, 0x0a,   17 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6,    2,    2,
    QMetaType::Void, 0x80000000 | 4,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int, 0x80000000 | 4, 0x80000000 | 6,   11,   12,
    QMetaType::Void, 0x80000000 | 6,   12,
    QMetaType::Void,

       0        // eod
};

void MyServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyServer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->s_started((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->s_connect((*reinterpret_cast< QHostAddress(*)>(_a[1]))); break;
        case 2: _t->s_read((*reinterpret_cast< QHostAddress(*)>(_a[1])),(*reinterpret_cast< NetworkData(*)>(_a[2]))); break;
        case 3: _t->s_disconnect((*reinterpret_cast< QHostAddress(*)>(_a[1]))); break;
        case 4: _t->slotNewConnection(); break;
        case 5: _t->slotServerRead(); break;
        case 6: { int _r = _t->slotWrite((*reinterpret_cast< const QHostAddress(*)>(_a[1])),(*reinterpret_cast< const NetworkData(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->slotsWrite((*reinterpret_cast< const NetworkData(*)>(_a[1]))); break;
        case 8: _t->slotClientDisconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MyServer::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyServer::s_started)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MyServer::*)(QHostAddress );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyServer::s_connect)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MyServer::*)(QHostAddress , NetworkData );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyServer::s_read)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MyServer::*)(QHostAddress );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyServer::s_disconnect)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject MyServer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MyServer.offsetsAndSize,
    qt_meta_data_MyServer,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MyServer_t
, QtPrivate::TypeAndForceComplete<MyServer, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>, QtPrivate::TypeAndForceComplete<NetworkData, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QHostAddress, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<const QHostAddress &, std::false_type>, QtPrivate::TypeAndForceComplete<const NetworkData &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const NetworkData &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *MyServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyServer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MyServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void MyServer::s_started(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MyServer::s_connect(QHostAddress _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MyServer::s_read(QHostAddress _t1, NetworkData _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MyServer::s_disconnect(QHostAddress _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
