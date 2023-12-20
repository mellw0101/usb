#include "AppAddDialog.h"
#include "IconButton.h"
#include "SimpleDock.h"
#include "dPM.hpp"
#include "debug.h"
#include "qaction.h"
#include "qmenu.h"
#include "windowManager.h"
#include "dbus.h"
#include "dockConfig.h"

#include <QMenu>
#include <QSettings>
#include <variant>
#include <vector>

std::string extractUUID(const std::string& input) {
    size_t start = input.find('{');
    size_t end = input.find('}');
    if (start != std::string::npos && end != std::string::npos && end > start) {
        return input.substr(start + 1, end - start - 1);
    }
    return "";
}

void SimpleDock::contextMenuEvent(QContextMenuEvent *event) {
    QMenu contextMenu(this);                            // Main ContextMenu

    QWidget *clickedWidget = childAt(event->pos());     // Only add "Remove Button" to contextMenu if right clicked on button
    IconButton *clickedButton = dynamic_cast<IconButton *>(clickedWidget);
    if (clickedButton) {

        caption = QStringList();
        uuid = QStringList();

        QString buttonName = clickedButton->getName(); // Retrieve the name of the button
        QString iconName = clickedButton->icon().name();
        debug(F, __func__, ": ", V, "buttonName: ", VV, buttonName, V, " iconName: ", VV, iconName, R);

        QStringList currentlyRunning = manager.getRunningProsseses(buttonName);

        matcher.match(manager.getCorrectProssesName(buttonName));
        if (!windowinfo.empty()) {
            for (int i = 0; i <= windowinfo.size() - 1; i += 1) {
                QStringList parts = windowinfo[i].split("=");
                for (int i = 0; i <= parts.size() - 1; i += 1) {
                    if (parts[i] == "windowId") {
                        std::string UUID = parts[i + 1].toStdString();
                        UUID = extractUUID(UUID);
                        getWindowInfo.callGetWindowInfo(QString::fromStdString(UUID));
                        // debug::output(parts[i + 1]);
                    }
                }
            }
        }
        debug(currentlyRunning);

        // debug::output(debug::QStringList_to_QString(windowinfo));

        if (!currentlyRunning.empty()) {
            QMenu *prossesesMenu = contextMenu.addMenu(buttonName);
            for (int i = 0; i <= currentlyRunning.size() - 1; i += 1) {
                QString num = QString::number(i);

                if (num == QString::number(0))
                    num = "";
                else
                    num = " <" + QString::number(i + 1) + ">";

                QString name = QString();
                if (i < caption.size())
                    name = caption[i];


                prossesesMenu->addAction("kill: " + name + " / " + currentlyRunning[i] + num, this, [this, currentlyRunning, i]() {
                    qDebug() << "\e[35mcontextMenuEvent\e[0m: Running killJob.";
                    killJob(currentlyRunning[i].toLongLong());
                });
            }
        }

        // Construct the action text
        QString actionText = QString("Remove %1 from dock").arg(buttonName.section("/", - 1));
        QAction *removeAction = contextMenu.addAction(actionText);
        connect(removeAction, &QAction::triggered, this, [this, clickedButton]() {
            qDebug() << "\e[35mcontextMenuEvent\e[0m: Running removeButton.";
            removeButton(clickedButton);
        });

        QAction* actionAddApp = contextMenu.addAction("Add App");
        connect(actionAddApp, &QAction::triggered, this, [this, clickedButton]() {
            openAppAddDialog();
        });

        QAction* moveLeftAction = contextMenu.addAction("Move Left");
        QAction* moveRightAction = contextMenu.addAction("Move Right");
        connect(moveLeftAction, &QAction::triggered, this, [this, clickedButton]() {
            qDebug() << "\e[35mcontextMenuEvent\e[0m: Running moveButton.";
            moveButton(clickedButton, MoveDirection::Left);
        });
        connect(moveRightAction, &QAction::triggered, this, [this, clickedButton]() {
            qDebug() << "\e[35mcontextMenuEvent\e[0m: Running moveButton.";
            moveButton(clickedButton, MoveDirection::Right);
        });
    }

    QMenu *prefrenceMenu = contextMenu.addMenu("Dock Prefrences");
    // Submenu for Placement
    QMenu *placementMenu = prefrenceMenu->addMenu("Placement");
    placementMenu->addAction("Top", this, [this]() { moveToEdge("Top"); });
    placementMenu->addAction("Bottom", this, [this]() { moveToEdge("Bottom"); });
    placementMenu->addAction("Left", this, [this]() { moveToEdge("Left"); });
    placementMenu->addAction("Right", this, [this]() { moveToEdge("Right"); });

    // Submenu for Button Size
    QMenu *sizeMenu = prefrenceMenu->addMenu("Button Size");
    for (int size = 40; size <= 80; size += 2) {
        QString sizeStr = QString::number(size);
        sizeMenu->addAction(sizeStr, this, [this, size]() {
            changeNum(ICON_SIZE, size);
            changeNum(BUTTON_SIZE, size);
            updateDockGeometry();
        });
    }

    // Submenu for Bounce Size
    QMenu *bounceSizeMenu = prefrenceMenu->addMenu("Bounce Size");
    for (int size = 4; size <= 20; size += 1) {
        QAction *action = bounceSizeMenu->addAction(QString::number(size));
        connect(action, &QAction::triggered, this, [this, size]() {// Connect the action to a lambda function
            changeNum(BOUNCE_ICON_SIZE, size);
            updateDockGeometry();
        });
    }

    // Submenu for Bounce Animation Duration
    QMenu *bounceAnimDurMenu = prefrenceMenu->addMenu("Bounce Animation Duration");
    for (int size = 20; size <= 400; size += 5) {
        QAction *action = bounceAnimDurMenu->addAction(QString::number(size));
        connect(action, &QAction::triggered, this, [this, size]() {// Connect the action to a lambda function
            changeNum(BOUNCE_ANIM_DUR, size);
        });
    }

    // Submenu for Zoom Animation Duration
    QMenu *zoomAnimDurMenu = prefrenceMenu->addMenu("Zoom Animation Duration");
    for (int size = 20; size <= 600; size += 5) {
        QAction *action = zoomAnimDurMenu->addAction(QString::number(size));
        connect(action, &QAction::triggered, this, [this, size]() {// Connect the action to a lambda function
            changeNum(ZOOM_ANIM_DUR, size);
        });
    }
    // Submenu for Zoom Scale
    QMenu *ZoomMenu = prefrenceMenu->addMenu("Zoom Scale");
    for (int size = 100; size <= 160; size += 5) {
        float scale = size / 100.0f;
        QAction *action = ZoomMenu->addAction(QString::number(scale));
        // Connect the action to a lambda function
        connect(action, &QAction::triggered, this, [this, size]() {
            changeNum(SCALE_FACTOR, size);
            updateDockGeometry();
        });
    }

    QMenu *squeezeMenu = prefrenceMenu->addMenu("Squeeze Menu");
    for (int size = 0; size <= 40; size += 1){
        QAction *action = squeezeMenu->addAction(QString::number(size));
        connect(action, &QAction::triggered, this, [this, size](){
           changeNum(SQUEEZE_NUM, size);
           updateDockGeometry();
        });
    }

    QAction *DEBUG = contextMenu.addAction("DEBUG");
    connect(DEBUG, &QAction::triggered, this, [this](){
       testDEBUG();
    });
    contextMenu.exec(event->globalPos());
}
