#ifndef SIMPLEDOCK_H
#define SIMPLEDOCK_H

#include "DockManager.hpp"
#include "dPM.hpp"
#include "IconButton.h"
#include "AppAddDialog.h"
#include "PidManager.h"
#include "dbus.h"
#include "debug.h"
#include "dockConfig.h"
#include "qcontainerfwd.h"
#include "qdockwidget.h"
#include "qmainwindow.h"
#include "qobject.h"
#include "qwidget.h"
#include <QMainWindow>


#include <QSettings>
#include <QObject>

class SimpleDock : public QWidget {
    Q_OBJECT
    // Add new members to handle settings
    QSettings settings;
private:
    // QHBoxLayout *layout;
    QHBoxLayout *mainLayout; // Main layout for the widget
    QSpacerItem *backSpacer = new QSpacerItem(10, num(BUTTON_SIZE), QSizePolicy::Expanding, QSizePolicy::Preferred);
    QList<IconButton*> buttons; // Use IconButton instead of QPushButton
    QPoint dragPosition; // to store the initial click position
    PidManager manager;
    DockManager dockManager;
    std::vector<pid_t> newPid = {};
    bool screenGeometryShown = false;
    DBusMessenger *messenger;
    DbusMatcher matcher;
    DbusGetWindowInfo getWindowInfo;
    QStringList windowinfo;
    QStringList caption;
    QStringList uuid;
    QMap<QString, QVector<QUuid>> uuidMap;
public slots:
    void sizeChangedRecived(const QVariant &num){
        double reply = num.toDouble();
        debug("Received sizeChangedSignal: ", reply);
    }
    QStringList DBusMatchReceived(const QVariant &reply) {
        if (!reply.canConvert<QList<MyDBusStruct>>()) {
            qDebug() << "Received reply is not of expected type";
            return QStringList();
        }
        QStringList info;
        windowinfo = QStringList();

        QList<MyDBusStruct> resultList = reply.value<QList<MyDBusStruct>>();
        for (const MyDBusStruct &item : resultList) {
            // qDebug() << "String1:" << item.string1;         // uuid with some code in front
            // qDebug() << "String2:" << item.string2;         // caption name
            caption.append(item.string2);
            // qDebug() << "String3:" << item.string3;
            // qDebug() << "UInt Value:" << item.uintVal;
            // qDebug() << "Double Value:" << item.doubleVal;
            // qDebug() << "Map Values:" << item.mapVal;
            // Process each item as needed
            windowinfo.append("windowId= " + item.string1 + " =windowTitle= " + item.string2 + " =iconName= " + item.string3);
            for (auto it = item.mapVal.constBegin(); it != item.mapVal.constEnd(); ++it) {
                // Convert the key and value to QString and add to the list
                QString key = it.key();
                QString value = it.value().toString(); // Assumes that the QVariant can be converted to a QString
                windowinfo.append("=" + key + "=" + value);
            }
        }
        // debug(F, "DBusMatchReceived", R, ": ", V, "windowinfo", R, ": ", VV, windowinfo, R);
        return info;
    }
    QStringList DbusGetWindowInfoReceived(const QVariantMap &map) {
        QStringList list;
        for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
            // Create a string representation of each key-value pair
            QString key = it.key();
            QString value = it.value().toString(); // Convert the QVariant value to a QString
            list.append(key + ": " + value); // Add the key-value pair to the list
            // qDebug() << "key:" << key << "value:" << value;
            // if (key == "caption") {
            //     debug(V, "caption: ", VV, value, R);
            //     caption.append(value);
            // }
            if (key == "uuid") {
                if (value.length() > 0) {
                    value.remove(0, 1);
                    value.chop(1);
                }
                debug(V, "uuid: ", VV, value, R);
                uuid.append(value);
            }
        }
        debug("caption: ", caption);
        debug("uuid: ", uuid);
        return list;
    }
    QStringList DbusGetAllRulesReceived(const QVariant &reply) {
        if (!reply.canConvert<QList<MyDBusStruct>>()) {
            qDebug() << "Received reply is not of expected type";
            return QStringList();
        }
        QStringList info;
        windowinfo = QStringList();

        QList<MyDBusStruct> resultList = reply.value<QList<MyDBusStruct>>();
            // qDebug() << "String1:" << item.string1;
            // qDebug() << "String2:" << item.string2;
            // qDebug() << "String3:" << item.string3;
            // qDebug() << "UInt Value:" << item.uintVal;
            // qDebug() << "Double Value:" << item.doubleVal;
            // qDebug() << "Map Values:" << item.mapVal;
            // // Process each item as needed
        for (const MyDBusStruct &item : resultList) {
            windowinfo.append("windowId= " + item.string1 + " =windowTitle= " + item.string2 + " =iconName= " + item.string3);
            for (auto it = item.mapVal.constBegin(); it != item.mapVal.constEnd(); ++it) {
                // Convert the key and value to QString and add to the list
                QString key = it.key();
                QString value = it.value().toString(); // Assumes that the QVariant can be converted to a QString
                DEBUG::output("=" + key + "=" + value);
            }
        }
        return QStringList();
    }
public:
    std::string session;
    QString current_placement = "Buttom";

    SimpleDock();

    void testDEBUG() {
        for (IconButton *button : buttons) {
            manager.updateDockManager(dockManager, button->appPath());
        }
        debug(dockManager.DATA());
    }

    void updateDockGeometry();
    void initializeButtons();
    void addButton(QHBoxLayout *layout, const QString &app);
    void removeButton(IconButton *button);
    void AddButtonRequested();
    void moveToEdge(const QString &edge);
    void moveToVirtualDesktop(int desktopNumber);
    QString findIconForBinary(const QString& binaryPath);
    QString findIconFilePath(const QString& iconName);
    void openAppAddDialog();
    QPixmap makePngScaleble(const QString& path);
    void saveButtonsToFile();
    void restoreButtonsFromFile();
    enum class MoveDirection {
        Left,
        Right
    };
    void moveButton(IconButton* button, MoveDirection direction);
    void killJob(const int &pid);
    void checkIfWayland();
    std::vector<pid_t> checkPid(const QString &prossesName, const int &parentPid);

    // Prevent copy and move semantics
    SimpleDock(const SimpleDock &) = delete;
    SimpleDock(SimpleDock &&) = delete;
    SimpleDock &operator=(const SimpleDock &) = delete;
    SimpleDock &operator=(SimpleDock &&) = delete;
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // SIMPLEDOCK_H
