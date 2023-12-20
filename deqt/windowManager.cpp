#include "SimpleDock.h"

#include "windowManager.h"
#include <QGuiApplication>
#include "debug.h"
#include "kwindowsystem.h"
#include "kx11extras.h"
#include "qcontainerfwd.h"
#include "qdbusreply.h"
#include <KX11Extras>
#include "qvariant.h"
#include <QTimer>
#include <QObject>
#include <cstdlib>
#include <string>

WindowManager::WindowManager() {
        // Setup the timer in the constructor
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &WindowManager::getCurrentDesktop);
        timer->start(1000); // Timer will trigger every 1000 ms (1 second)
    }

void WindowManager::getCurrentDesktop() {
    QDBusInterface kwin("org.kde.KWin", "/KWin", "org.kde.KWin", QDBusConnection::sessionBus());
    QDBusReply<int> reply = kwin.call("currentDesktop");
    if (reply.isValid()) {
        DEBUG::output("getCurrentDesktop: reply(valid): currentDesktop: " + QString::number(reply));
        return;
    }
    qDebug() << "Error: " << reply.error().message();
}
int WindowManager::currentDesktop() {
    return desktop;
}

