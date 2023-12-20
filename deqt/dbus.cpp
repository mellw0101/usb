#include "dbus.h"
#include "qcontainerfwd.h"
#include "qobject.h"
#include "qtmetamacros.h"
#include "qvariant.h"
#include <QDBusMessage>
#include <QDBusConnection>

DBusMessenger::DBusMessenger(KWIN::Config config, QObject *parent) : QObject(parent), dbusInterface(nullptr) {
    if (config == KWIN::SCRIPTING) {
        dbusInterface = new QDBusInterface("org.kde.KWin", "/Scripting", "", QDBusConnection::sessionBus(), this);
    }
    // Add more cases as necessary
}

DBusMessenger::DBusMessenger(PredefinedConfig config, QObject *parent) : QObject(parent), dbusInterface(nullptr) {
    switch (config) {
        case KWIN_SCRIPTING:
            dbusInterface = new QDBusInterface("org.kde.KWin", "/Scripting", "", QDBusConnection::sessionBus(), this);
            break;
        // Handle other cases if you have more predefined configurations
    }
}

DBusMessenger::DBusMessenger(const QString &service, const QString &path, const QString &interface, QObject *parent)
    : QObject(parent), dbusInterface(new QDBusInterface(service, path, interface, QDBusConnection::sessionBus(), this)) {}

DBusMessenger::~DBusMessenger() {
    delete dbusInterface;
}
// void DBusMessenger::sendMessage(SCRIPTING::Methods method, const QString &message) {
//     if (method == SCRIPTING::ISSCRIPTLOADED) {
//         QDBusMessage msg = dbusInterface->call("isScriptLoaded", message);
//         if (msg.type() == QDBusMessage::ReplyMessage) {
//             QStringList stringList;
//             for (const QVariant &arg : msg.arguments()) {
//                 stringList.append(arg.toString());
//             }
//             emit signalReplyReceived(stringList.join(", "));
//         } else {
//             // Handle error or asynchronous reply
//             // You might want to emit a different signal for errors
//         }
//     }
// }


void DBusMessenger::sendMessage(const QString &methodName, const QString &message) {
    QDBusMessage msg = dbusInterface->call(methodName, message);
    if (msg.type() == QDBusMessage::ReplyMessage) {
        QStringList stringList;
        for (const QVariant &arg : msg.arguments()) {
            stringList.append(arg.toString());
        }
        emit signalReplyReceived(stringList.join(", "));
    } else {
        // Handle error or asynchronous reply
        // You might want to emit a different signal for errors
    }
}
void DBusMessenger::sendMessage(const QString &methodName, const QVariantList &rules) {
    QDBusMessage msg = dbusInterface->call(methodName, rules);
    if (msg.type() == QDBusMessage::ReplyMessage) {
        QStringList stringList;
        for (const QVariant &arg : msg.arguments()) {
            stringList.append(arg.toString());
        }
        emit signalReplyReceived(stringList.join(", "));
    } else {
        // Handle error or asynchronous reply
        // You might want to emit a different signal for errors
    }
}

#include <QDBusArgument>

const QDBusArgument &operator>>(const QDBusArgument &argument, MyDBusStruct &myStruct) {
    argument.beginStructure();
    argument >> myStruct.string1 >> myStruct.string2 >> myStruct.string3
             >> myStruct.uintVal >> myStruct.doubleVal >> myStruct.mapVal;
    argument.endStructure();
    return argument;
}

DbusMatcher::DbusMatcher(QObject *parent)
    : QObject(parent), dbusInterface("org.kde.KWin", "/WindowsRunner", "", QDBusConnection::sessionBus()) {}

void DbusMatcher::match(const QString &query) {
    qRegisterMetaType<MyDBusStruct>("MyDBusStruct");
    QDBusMessage response = dbusInterface.call("Match", query);
    if (response.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Error calling Match method:" << response.errorMessage();
        emit matchResult(QVariant()); // Emit an empty QVariant or an error signal
        return;
    }

    const QDBusArgument &dbusArg = response.arguments().at(0).value<QDBusArgument>();
    QList<MyDBusStruct> resultList;
    dbusArg >> resultList;
    emit matchResult(QVariant::fromValue(resultList));
}

DbusGetWindowInfo::DbusGetWindowInfo(QObject *parent)
    : QObject(parent), dbusInterface("org.kde.KWin", "/KWin", "", QDBusConnection::sessionBus()) {}

void DbusGetWindowInfo::callGetWindowInfo(const QString &windowId) {
    QDBusMessage response = dbusInterface.call("getWindowInfo", windowId);

    if (response.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Error calling getWindowInfo:" << response.errorMessage();
        emit getWindowInfoResult(QVariantMap());
        return;                                             // Return an empty map in case of error
    }

    QVariantMap resultMap = qdbus_cast<QVariantMap>(response.arguments().at(0));
    emit getWindowInfoResult(resultMap);
}

DbusFreeDesktop::DbusFreeDesktop(QObject *parent)
    : QObject(parent), dbusInterface("org.freedesktop.DBus", "/org/freedesktop/DBus", "", QDBusConnection::sessionBus()){}

void DbusFreeDesktop::getAllMatchRules() {
    qRegisterMetaType<MyDBusStruct>("MyDBusStruct");
    QDBusMessage response = dbusInterface.call("GetAllMatchRules");
    if (response.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Error calling Match method:" << response.errorMessage();
        // emit matchResult(QVariant()); // Emit an empty QVariant or an error signal
        return;
    }

    const QDBusArgument &dbusArg = response.arguments().at(0).value<QDBusArgument>();
    QList<MyDBusStruct> resultList;
    dbusArg >> resultList;
    emit getAllMatchRulesResult(QVariant::fromValue(resultList));
}

