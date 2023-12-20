/****************************************************************************
** Meta object code from reading C++ file 'SimpleDock.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "SimpleDock.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SimpleDock.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.1. It"
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
struct qt_meta_stringdata_CLASSSimpleDockENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSSimpleDockENDCLASS = QtMocHelpers::stringData(
    "SimpleDock",
    "sizeChangedRecived",
    "",
    "num",
    "DBusMatchReceived",
    "reply",
    "DbusGetWindowInfoReceived",
    "map",
    "DbusGetAllRulesReceived"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSSimpleDockENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[11];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[4];
    char stringdata4[18];
    char stringdata5[6];
    char stringdata6[26];
    char stringdata7[4];
    char stringdata8[24];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSSimpleDockENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSSimpleDockENDCLASS_t qt_meta_stringdata_CLASSSimpleDockENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "SimpleDock"
        QT_MOC_LITERAL(11, 18),  // "sizeChangedRecived"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 3),  // "num"
        QT_MOC_LITERAL(35, 17),  // "DBusMatchReceived"
        QT_MOC_LITERAL(53, 5),  // "reply"
        QT_MOC_LITERAL(59, 25),  // "DbusGetWindowInfoReceived"
        QT_MOC_LITERAL(85, 3),  // "map"
        QT_MOC_LITERAL(89, 23)   // "DbusGetAllRulesReceived"
    },
    "SimpleDock",
    "sizeChangedRecived",
    "",
    "num",
    "DBusMatchReceived",
    "reply",
    "DbusGetWindowInfoReceived",
    "map",
    "DbusGetAllRulesReceived"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSimpleDockENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x0a,    1 /* Public */,
       4,    1,   41,    2, 0x0a,    3 /* Public */,
       6,    1,   44,    2, 0x0a,    5 /* Public */,
       8,    1,   47,    2, 0x0a,    7 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant,    3,
    QMetaType::QStringList, QMetaType::QVariant,    5,
    QMetaType::QStringList, QMetaType::QVariantMap,    7,
    QMetaType::QStringList, QMetaType::QVariant,    5,

       0        // eod
};

Q_CONSTINIT const QMetaObject SimpleDock::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSSimpleDockENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSimpleDockENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSimpleDockENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SimpleDock, std::true_type>,
        // method 'sizeChangedRecived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        // method 'DBusMatchReceived'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>,
        // method 'DbusGetWindowInfoReceived'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        // method 'DbusGetAllRulesReceived'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>
    >,
    nullptr
} };

void SimpleDock::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SimpleDock *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sizeChangedRecived((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1]))); break;
        case 1: { QStringList _r = _t->DBusMatchReceived((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 2: { QStringList _r = _t->DbusGetWindowInfoReceived((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 3: { QStringList _r = _t->DbusGetAllRulesReceived((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *SimpleDock::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SimpleDock::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSimpleDockENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SimpleDock::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
