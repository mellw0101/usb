#include "dbus.h"
#include "qcontainerfwd.h"

dbus_kwinScripting::dbus_kwinScripting(QObject *parent)
    : DBusMessenger("org.kde.KWin", "/Scripting", "", parent) {}

void dbus_kwinScripting::isScriptLoaded(const QString &filePath) {
    sendMessage("isScriptLoaded", filePath);
}
void dbus_kwinScripting::loadDeclarativeScript(const QString &filePath){
    sendMessage("loadDeclarativeScript", filePath);
}
void dbus_kwinScripting::unloadScript(const QString &filePath) {
    sendMessage("unloadScript", filePath);
}

dbus_kwinWindowsRunner::dbus_kwinWindowsRunner(QObject *parent)
    : DBusMessenger("org.kde.KWin", "/WindowsRunner", "", parent) {}

void dbus_kwinWindowsRunner::Match(const QString &programName) {
    sendMessage("Match", programName);
}

dbus_kwinRules::dbus_kwinRules(QObject *parent)
    : DBusMessenger("org.kde.KWin", "/KWin", "org.kde.KWin.rules", parent) {}

void dbus_kwinRules::setRules(const QVariantList &rules) {
    sendMessage("setRule", rules);
}
