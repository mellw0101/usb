/****************************************************************************
** Meta object code from reading C++ file 'wCompositor.hpp'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../wCompositor.hpp"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wCompositor.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSWCompoENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSWCompoENDCLASS = QtMocHelpers::stringData(
    "WCompo",
    "requestMoveWindow",
    "",
    "x",
    "y",
    "onNewClient",
    "QWaylandClient*",
    "client",
    "signalRecived"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSWCompoENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[7];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[2];
    char stringdata4[2];
    char stringdata5[12];
    char stringdata6[16];
    char stringdata7[7];
    char stringdata8[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSWCompoENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSWCompoENDCLASS_t qt_meta_stringdata_CLASSWCompoENDCLASS = {
    {
        QT_MOC_LITERAL(0, 6),  // "WCompo"
        QT_MOC_LITERAL(7, 17),  // "requestMoveWindow"
        QT_MOC_LITERAL(25, 0),  // ""
        QT_MOC_LITERAL(26, 1),  // "x"
        QT_MOC_LITERAL(28, 1),  // "y"
        QT_MOC_LITERAL(30, 11),  // "onNewClient"
        QT_MOC_LITERAL(42, 15),  // "QWaylandClient*"
        QT_MOC_LITERAL(58, 6),  // "client"
        QT_MOC_LITERAL(65, 13)   // "signalRecived"
    },
    "WCompo",
    "requestMoveWindow",
    "",
    "x",
    "y",
    "onNewClient",
    "QWaylandClient*",
    "client",
    "signalRecived"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSWCompoENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   32,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   37,    2, 0x09,    4 /* Protected */,
       8,    0,   40,    2, 0x09,    6 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject WCompo::staticMetaObject = { {
    QMetaObject::SuperData::link<QWaylandCompositor::staticMetaObject>(),
    qt_meta_stringdata_CLASSWCompoENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSWCompoENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSWCompoENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<WCompo, std::true_type>,
        // method 'requestMoveWindow'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onNewClient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QWaylandClient *, std::false_type>,
        // method 'signalRecived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void WCompo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WCompo *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestMoveWindow((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->onNewClient((*reinterpret_cast< std::add_pointer_t<QWaylandClient*>>(_a[1]))); break;
        case 2: _t->signalRecived(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWaylandClient* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WCompo::*)(int , int );
            if (_t _q_method = &WCompo::requestMoveWindow; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *WCompo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WCompo::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSWCompoENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWaylandCompositor::qt_metacast(_clname);
}

int WCompo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWaylandCompositor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void WCompo::requestMoveWindow(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
