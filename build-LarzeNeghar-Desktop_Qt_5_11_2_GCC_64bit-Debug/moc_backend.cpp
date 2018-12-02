/****************************************************************************
** Meta object code from reading C++ file 'backend.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../LarzeNeghar/backend.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'backend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BackEnd_t {
    QByteArrayData data[26];
    char stringdata0[303];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BackEnd_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BackEnd_t qt_meta_stringdata_BackEnd = {
    {
QT_MOC_LITERAL(0, 0, 7), // "BackEnd"
QT_MOC_LITERAL(1, 8, 11), // "updateChart"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 16), // "QAbstractSeries*"
QT_MOC_LITERAL(4, 38, 11), // "chartSeries"
QT_MOC_LITERAL(5, 50, 12), // "setAxisXTime"
QT_MOC_LITERAL(6, 63, 14), // "QDateTimeAxis*"
QT_MOC_LITERAL(7, 78, 4), // "axis"
QT_MOC_LITERAL(8, 83, 12), // "setAxisYTime"
QT_MOC_LITERAL(9, 96, 12), // "setAxisZTime"
QT_MOC_LITERAL(10, 109, 10), // "appendItem"
QT_MOC_LITERAL(11, 120, 9), // "timerSlot"
QT_MOC_LITERAL(12, 130, 17), // "recieveSerialPort"
QT_MOC_LITERAL(13, 148, 14), // "setSensorsList"
QT_MOC_LITERAL(14, 163, 12), // "SensorsList*"
QT_MOC_LITERAL(15, 176, 11), // "sensorsList"
QT_MOC_LITERAL(16, 188, 14), // "getSensorsList"
QT_MOC_LITERAL(17, 203, 15), // "QVector<Sensor>"
QT_MOC_LITERAL(18, 219, 10), // "getSenorsX"
QT_MOC_LITERAL(19, 230, 10), // "QList<int>"
QT_MOC_LITERAL(20, 241, 12), // "routerNumber"
QT_MOC_LITERAL(21, 254, 12), // "sensorNumber"
QT_MOC_LITERAL(22, 267, 6), // "bordar"
QT_MOC_LITERAL(23, 274, 10), // "getSenorsY"
QT_MOC_LITERAL(24, 285, 4), // "test"
QT_MOC_LITERAL(25, 290, 12) // "sendSettings"

    },
    "BackEnd\0updateChart\0\0QAbstractSeries*\0"
    "chartSeries\0setAxisXTime\0QDateTimeAxis*\0"
    "axis\0setAxisYTime\0setAxisZTime\0"
    "appendItem\0timerSlot\0recieveSerialPort\0"
    "setSensorsList\0SensorsList*\0sensorsList\0"
    "getSensorsList\0QVector<Sensor>\0"
    "getSenorsX\0QList<int>\0routerNumber\0"
    "sensorNumber\0bordar\0getSenorsY\0test\0"
    "sendSettings"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BackEnd[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x0a /* Public */,
       5,    1,   82,    2, 0x0a /* Public */,
       8,    1,   85,    2, 0x0a /* Public */,
       9,    1,   88,    2, 0x0a /* Public */,
      10,    0,   91,    2, 0x0a /* Public */,
      11,    0,   92,    2, 0x0a /* Public */,
      12,    0,   93,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
      13,    1,   94,    2, 0x02 /* Public */,
      16,    0,   97,    2, 0x02 /* Public */,
      18,    3,   98,    2, 0x02 /* Public */,
      23,    3,  105,    2, 0x02 /* Public */,
      24,    0,  112,    2, 0x02 /* Public */,
      25,    0,  113,    2, 0x02 /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, 0x80000000 | 14,   15,
    0x80000000 | 17,
    0x80000000 | 19, QMetaType::QString, QMetaType::QString, QMetaType::QString,   20,   21,   22,
    0x80000000 | 19, QMetaType::QString, QMetaType::QString, QMetaType::QString,   20,   21,   22,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BackEnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BackEnd *_t = static_cast<BackEnd *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateChart((*reinterpret_cast< QAbstractSeries*(*)>(_a[1]))); break;
        case 1: _t->setAxisXTime((*reinterpret_cast< QDateTimeAxis*(*)>(_a[1]))); break;
        case 2: _t->setAxisYTime((*reinterpret_cast< QDateTimeAxis*(*)>(_a[1]))); break;
        case 3: _t->setAxisZTime((*reinterpret_cast< QDateTimeAxis*(*)>(_a[1]))); break;
        case 4: _t->appendItem(); break;
        case 5: _t->timerSlot(); break;
        case 6: _t->recieveSerialPort(); break;
        case 7: _t->setSensorsList((*reinterpret_cast< SensorsList*(*)>(_a[1]))); break;
        case 8: { QVector<Sensor> _r = _t->getSensorsList();
            if (_a[0]) *reinterpret_cast< QVector<Sensor>*>(_a[0]) = std::move(_r); }  break;
        case 9: { QList<int> _r = _t->getSenorsX((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QList<int>*>(_a[0]) = std::move(_r); }  break;
        case 10: { QList<int> _r = _t->getSenorsY((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< QList<int>*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->test(); break;
        case 12: _t->sendSettings(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSeries* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QDateTimeAxis* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QDateTimeAxis* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QDateTimeAxis* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SensorsList* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BackEnd::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BackEnd.data,
      qt_meta_data_BackEnd,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BackEnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BackEnd::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BackEnd.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BackEnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
