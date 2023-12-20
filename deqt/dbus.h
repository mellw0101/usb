#ifndef DBUS_H
#define DBUS_H

#include "qcontainerfwd.h"
#include "qwidget.h"
#include <QList>
#include <QVariant>
#include <QVariantList>
#include <QObject>
#include <QDBusInterface>
#include <QCoreApplication>

// qdbus --literal org.freedesktop.DBus /org/freedesktop/DBus GetAllMatchRules


namespace KWIN {
    enum Config {
        SCRIPTING
        // Other configurations specific to KWIN can be added here
    };
}
namespace SCRIPTING {
    enum Methods {
        ISSCRIPTLOADED
    };
}

class DBusMessenger : public QObject {
    Q_OBJECT

    public:
        enum PredefinedConfig {
            KWIN_SCRIPTING
            // Add other predefined configurations here
        };

        DBusMessenger(KWIN::Config config, QObject *parent = nullptr);
        DBusMessenger(PredefinedConfig config, QObject *parent = nullptr);
        DBusMessenger(const QString &service, const QString &path, const QString &interface, QObject *parent = nullptr);
        virtual ~DBusMessenger();

        virtual void sendMessage(const QString &methodName, const QString &message);
        virtual void sendMessage(const QString &methodName, const QVariantList &rules);
        // ~DBusMessenger();

        // void sendMessage(SCRIPTING::Methods method, const QString &message);
        // void sendMessage(const QString &methodName, const QVariantList &rules);
        // // void sendMessage(const QString &methodName, const QString &message);

    signals:
        void signalReplyReceived(const QString &reply);

    private:
        QDBusInterface *dbusInterface;
};

class dbus_kwinScripting : public DBusMessenger {
    Q_OBJECT
public:
    dbus_kwinScripting(QObject *parent = nullptr);
    void isScriptLoaded(const QString &filePath);
    void loadDeclarativeScript(const QString &filePath);
    void unloadScript(const QString &filePath);
};
class dbus_kwinWindowsRunner : public DBusMessenger {
    Q_OBJECT
public:
    dbus_kwinWindowsRunner(QObject *parent = nullptr);
    void Match(const QString &programName);
};
class dbus_kwinRules : public DBusMessenger {
    Q_OBJECT
public:
    dbus_kwinRules(QObject *parent = nullptr);
    void setRules(const QVariantList &rules);
};

#include <QMetaType>
#include <QString>
#include <QMap>
#include <QVariant>

struct MyDBusStruct {
    QString string1;
    QString string2;
    QString string3;
    quint32 uintVal;
    double doubleVal;
    QMap<QString, QVariant> mapVal;
};
Q_DECLARE_METATYPE(MyDBusStruct)



class DbusMatcher : public QObject {
    Q_OBJECT

public:
    DbusMatcher(QObject *parent = nullptr);
    void match(const QString &query);

signals:
    void matchResult(const QVariant &result);

private:
    QDBusInterface dbusInterface;
};

class DbusGetWindowInfo : public QObject {
    Q_OBJECT

public:
    DbusGetWindowInfo(QObject *parent = nullptr);
    void callGetWindowInfo(const QString &windowId);

signals:
    void getWindowInfoResult(const QVariantMap &result);

private:
    QDBusInterface dbusInterface;
};

class DbusFreeDesktop : public QObject {
    Q_OBJECT

public:
    DbusFreeDesktop(QObject *parent = nullptr);
    void getAllMatchRules();

signals:
    void getAllMatchRulesResult(const QVariant &result);

private:
    QDBusInterface dbusInterface;
};


#endif // DBUS_H
