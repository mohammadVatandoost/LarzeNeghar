/****************************************************************************
** Meta object code from reading C++ file 'sensorslist.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../LarzeNeghar/sensorslist.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sensorslist.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SensorsList_t {
    QByteArrayData data[22];
    char stringdata0[277];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SensorsList_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SensorsList_t qt_meta_stringdata_SensorsList = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SensorsList"
QT_MOC_LITERAL(1, 12, 15), // "preItemAppended"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 16), // "postItemAppended"
QT_MOC_LITERAL(4, 46, 14), // "preItemRemoved"
QT_MOC_LITERAL(5, 61, 5), // "index"
QT_MOC_LITERAL(6, 67, 15), // "postItemRemoved"
QT_MOC_LITERAL(7, 83, 10), // "appendItem"
QT_MOC_LITERAL(8, 94, 10), // "removeItem"
QT_MOC_LITERAL(9, 105, 13), // "getSensorXmin"
QT_MOC_LITERAL(10, 119, 13), // "getSensorXmax"
QT_MOC_LITERAL(11, 133, 13), // "getSensorYmin"
QT_MOC_LITERAL(12, 147, 13), // "getSensorYmax"
QT_MOC_LITERAL(13, 161, 13), // "getSensorZmin"
QT_MOC_LITERAL(14, 175, 13), // "getSensorZmax"
QT_MOC_LITERAL(15, 189, 8), // "zoomPlus"
QT_MOC_LITERAL(16, 198, 6), // "bordar"
QT_MOC_LITERAL(17, 205, 9), // "zoomMines"
QT_MOC_LITERAL(18, 215, 14), // "scrollDataPlus"
QT_MOC_LITERAL(19, 230, 15), // "scrollDataMines"
QT_MOC_LITERAL(20, 246, 14), // "scrollTimePlus"
QT_MOC_LITERAL(21, 261, 15) // "scrollTimeMines"

    },
    "SensorsList\0preItemAppended\0\0"
    "postItemAppended\0preItemRemoved\0index\0"
    "postItemRemoved\0appendItem\0removeItem\0"
    "getSensorXmin\0getSensorXmax\0getSensorYmin\0"
    "getSensorYmax\0getSensorZmin\0getSensorZmax\0"
    "zoomPlus\0bordar\0zoomMines\0scrollDataPlus\0"
    "scrollDataMines\0scrollTimePlus\0"
    "scrollTimeMines"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SensorsList[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x06 /* Public */,
       3,    0,  105,    2, 0x06 /* Public */,
       4,    1,  106,    2, 0x06 /* Public */,
       6,    0,  109,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,  110,    2, 0x0a /* Public */,
       8,    1,  111,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
       9,    0,  114,    2, 0x02 /* Public */,
      10,    0,  115,    2, 0x02 /* Public */,
      11,    0,  116,    2, 0x02 /* Public */,
      12,    0,  117,    2, 0x02 /* Public */,
      13,    0,  118,    2, 0x02 /* Public */,
      14,    0,  119,    2, 0x02 /* Public */,
      15,    1,  120,    2, 0x02 /* Public */,
      17,    1,  123,    2, 0x02 /* Public */,
      18,    1,  126,    2, 0x02 /* Public */,
      19,    1,  129,    2, 0x02 /* Public */,
      20,    1,  132,    2, 0x02 /* Public */,
      21,    1,  135,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,

 // methods: parameters
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString,   16,

       0        // eod
};

void SensorsList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SensorsList *_t = static_cast<SensorsList *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->preItemAppended(); break;
        case 1: _t->postItemAppended(); break;
        case 2: _t->preItemRemoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->postItemRemoved(); break;
        case 4: _t->appendItem(); break;
        case 5: _t->removeItem((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: { int _r = _t->getSensorXmin();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 7: { int _r = _t->getSensorXmax();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 8: { int _r = _t->getSensorYmin();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { int _r = _t->getSensorYmax();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 10: { int _r = _t->getSensorZmin();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 11: { int _r = _t->getSensorZmax();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->zoomPlus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->zoomMines((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->scrollDataPlus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->scrollDataMines((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->scrollTimePlus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->scrollTimeMines((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SensorsList::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorsList::preItemAppended)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SensorsList::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorsList::postItemAppended)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SensorsList::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorsList::preItemRemoved)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SensorsList::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SensorsList::postItemRemoved)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SensorsList::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_SensorsList.data,
    qt_meta_data_SensorsList,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SensorsList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SensorsList::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SensorsList.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SensorsList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void SensorsList::preItemAppended()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SensorsList::postItemAppended()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SensorsList::preItemRemoved(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SensorsList::postItemRemoved()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
