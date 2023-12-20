/****************************************************************************
** Meta object code from reading C++ file 'dbus.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dbus.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dbus.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSDBusMessengerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSDBusMessengerENDCLASS = QtMocHelpers::stringData(
    "DBusMessenger",
    "signalReplyReceived",
    "",
    "reply"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSDBusMessengerENDCLASS_t {
    uint offsetsAndSizes[8];
    char stringdata0[14];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSDBusMessengerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSDBusMessengerENDCLASS_t qt_meta_stringdata_CLASSDBusMessengerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 13),  // "DBusMessenger"
        QT_MOC_LITERAL(14, 19),  // "signalReplyReceived"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 5)   // "reply"
    },
    "DBusMessenger",
    "signalReplyReceived",
    "",
    "reply"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSDBusMessengerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject DBusMessenger::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSDBusMessengerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSDBusMessengerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSDBusMessengerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DBusMessenger, std::true_type>,
        // method 'signalReplyReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void DBusMessenger::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DBusMessenger *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signalReplyReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DBusMessenger::*)(const QString & );
            if (_t _q_method = &DBusMessenger::signalReplyReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *DBusMessenger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DBusMessenger::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSDBusMessengerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DBusMessenger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void DBusMessenger::signalReplyReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSdbus_kwinScriptingENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSdbus_kwinScriptingENDCLASS = QtMocHelpers::stringData(
    "dbus_kwinScripting"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSdbus_kwinScriptingENDCLASS_t {
    uint offsetsAndSizes[2];
    char stringdata0[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSdbus_kwinScriptingENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSdbus_kwinScriptingENDCLASS_t qt_meta_stringdata_CLASSdbus_kwinScriptingENDCLASS = {
    {
        QT_MOC_LITERAL(0, 18)   // "dbus_kwinScripting"
    },
    "dbus_kwinScripting"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSdbus_kwinScriptingENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject dbus_kwinScripting::staticMetaObject = { {
    QMetaObject::SuperData::link<DBusMessenger::staticMetaObject>(),
    qt_meta_stringdata_CLASSdbus_kwinScriptingENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSdbus_kwinScriptingENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSdbus_kwinScriptingENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<dbus_kwinScripting, std::true_type>
    >,
    nullptr
} };

void dbus_kwinScripting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *dbus_kwinScripting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dbus_kwinScripting::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSdbus_kwinScriptingENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return DBusMessenger::qt_metacast(_clname);
}

int dbus_kwinScripting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DBusMessenger::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSdbus_kwinWindowsRunnerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSdbus_kwinWindowsRunnerENDCLASS = QtMocHelpers::stringData(
    "dbus_kwinWindowsRunner"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSdbus_kwinWindowsRunnerENDCLASS_t {
    uint offsetsAndSizes[2];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSdbus_kwinWindowsRunnerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSdbus_kwinWindowsRunnerENDCLASS_t qt_meta_stringdata_CLASSdbus_kwinWindowsRunnerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 22)   // "dbus_kwinWindowsRunner"
    },
    "dbus_kwinWindowsRunner"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSdbus_kwinWindowsRunnerENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject dbus_kwinWindowsRunner::staticMetaObject = { {
    QMetaObject::SuperData::link<DBusMessenger::staticMetaObject>(),
    qt_meta_stringdata_CLASSdbus_kwinWindowsRunnerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSdbus_kwinWindowsRunnerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSdbus_kwinWindowsRunnerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<dbus_kwinWindowsRunner, std::true_type>
    >,
    nullptr
} };

void dbus_kwinWindowsRunner::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *dbus_kwinWindowsRunner::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dbus_kwinWindowsRunner::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSdbus_kwinWindowsRunnerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return DBusMessenger::qt_metacast(_clname);
}

int dbus_kwinWindowsRunner::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DBusMessenger::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSdbus_kwinRulesENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSdbus_kwinRulesENDCLASS = QtMocHelpers::stringData(
    "dbus_kwinRules"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSdbus_kwinRulesENDCLASS_t {
    uint offsetsAndSizes[2];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSdbus_kwinRulesENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSdbus_kwinRulesENDCLASS_t qt_meta_stringdata_CLASSdbus_kwinRulesENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14)   // "dbus_kwinRules"
    },
    "dbus_kwinRules"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSdbus_kwinRulesENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

Q_CONSTINIT const QMetaObject dbus_kwinRules::staticMetaObject = { {
    QMetaObject::SuperData::link<DBusMessenger::staticMetaObject>(),
    qt_meta_stringdata_CLASSdbus_kwinRulesENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSdbus_kwinRulesENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSdbus_kwinRulesENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<dbus_kwinRules, std::true_type>
    >,
    nullptr
} };

void dbus_kwinRules::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject *dbus_kwinRules::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dbus_kwinRules::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSdbus_kwinRulesENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return DBusMessenger::qt_metacast(_clname);
}

int dbus_kwinRules::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DBusMessenger::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSDbusMatcherENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSDbusMatcherENDCLASS = QtMocHelpers::stringData(
    "DbusMatcher",
    "matchResult",
    "",
    "result"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSDbusMatcherENDCLASS_t {
    uint offsetsAndSizes[8];
    char stringdata0[12];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSDbusMatcherENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSDbusMatcherENDCLASS_t qt_meta_stringdata_CLASSDbusMatcherENDCLASS = {
    {
        QT_MOC_LITERAL(0, 11),  // "DbusMatcher"
        QT_MOC_LITERAL(12, 11),  // "matchResult"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 6)   // "result"
    },
    "DbusMatcher",
    "matchResult",
    "",
    "result"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSDbusMatcherENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariant,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject DbusMatcher::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSDbusMatcherENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSDbusMatcherENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSDbusMatcherENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DbusMatcher, std::true_type>,
        // method 'matchResult'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>
    >,
    nullptr
} };

void DbusMatcher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DbusMatcher *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->matchResult((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DbusMatcher::*)(const QVariant & );
            if (_t _q_method = &DbusMatcher::matchResult; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *DbusMatcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DbusMatcher::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSDbusMatcherENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DbusMatcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void DbusMatcher::matchResult(const QVariant & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSDbusGetWindowInfoENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSDbusGetWindowInfoENDCLASS = QtMocHelpers::stringData(
    "DbusGetWindowInfo",
    "getWindowInfoResult",
    "",
    "result"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSDbusGetWindowInfoENDCLASS_t {
    uint offsetsAndSizes[8];
    char stringdata0[18];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSDbusGetWindowInfoENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSDbusGetWindowInfoENDCLASS_t qt_meta_stringdata_CLASSDbusGetWindowInfoENDCLASS = {
    {
        QT_MOC_LITERAL(0, 17),  // "DbusGetWindowInfo"
        QT_MOC_LITERAL(18, 19),  // "getWindowInfoResult"
        QT_MOC_LITERAL(38, 0),  // ""
        QT_MOC_LITERAL(39, 6)   // "result"
    },
    "DbusGetWindowInfo",
    "getWindowInfoResult",
    "",
    "result"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSDbusGetWindowInfoENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariantMap,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject DbusGetWindowInfo::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSDbusGetWindowInfoENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSDbusGetWindowInfoENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSDbusGetWindowInfoENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DbusGetWindowInfo, std::true_type>,
        // method 'getWindowInfoResult'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>
    >,
    nullptr
} };

void DbusGetWindowInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DbusGetWindowInfo *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->getWindowInfoResult((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DbusGetWindowInfo::*)(const QVariantMap & );
            if (_t _q_method = &DbusGetWindowInfo::getWindowInfoResult; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *DbusGetWindowInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DbusGetWindowInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSDbusGetWindowInfoENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DbusGetWindowInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void DbusGetWindowInfo::getWindowInfoResult(const QVariantMap & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSDbusFreeDesktopENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSDbusFreeDesktopENDCLASS = QtMocHelpers::stringData(
    "DbusFreeDesktop",
    "getAllMatchRulesResult",
    "",
    "result"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSDbusFreeDesktopENDCLASS_t {
    uint offsetsAndSizes[8];
    char stringdata0[16];
    char stringdata1[23];
    char stringdata2[1];
    char stringdata3[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSDbusFreeDesktopENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSDbusFreeDesktopENDCLASS_t qt_meta_stringdata_CLASSDbusFreeDesktopENDCLASS = {
    {
        QT_MOC_LITERAL(0, 15),  // "DbusFreeDesktop"
        QT_MOC_LITERAL(16, 22),  // "getAllMatchRulesResult"
        QT_MOC_LITERAL(39, 0),  // ""
        QT_MOC_LITERAL(40, 6)   // "result"
    },
    "DbusFreeDesktop",
    "getAllMatchRulesResult",
    "",
    "result"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSDbusFreeDesktopENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   20,    2, 0x06,    1 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariant,    3,

       0        // eod
};

Q_CONSTINIT const QMetaObject DbusFreeDesktop::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSDbusFreeDesktopENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSDbusFreeDesktopENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSDbusFreeDesktopENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DbusFreeDesktop, std::true_type>,
        // method 'getAllMatchRulesResult'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariant &, std::false_type>
    >,
    nullptr
} };

void DbusFreeDesktop::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DbusFreeDesktop *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->getAllMatchRulesResult((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DbusFreeDesktop::*)(const QVariant & );
            if (_t _q_method = &DbusFreeDesktop::getAllMatchRulesResult; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *DbusFreeDesktop::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DbusFreeDesktop::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSDbusFreeDesktopENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DbusFreeDesktop::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void DbusFreeDesktop::getAllMatchRulesResult(const QVariant & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
