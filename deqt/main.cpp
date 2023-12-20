#include "SimpleDock.h"
#include "debug.h"
#include "qdebug.h"
#include "waylandClient.h"
#include "dbus.h"
#include "dockConfig.h"


#include <QApplication>
#include <QString>
#include <QGuiApplication>
#include "qwidget.h"
#include "qwindowdefs.h"
#include "windowManager.h"
#include <kwindowinfo.h>
#include <KX11Extras>
#include <KWindowSystem>


int main(int argc, char *argv[]) {
    initializeVars();
    setenv("QT_QPA_PLATFORM", "xcb", 1);
    QApplication app(argc, argv);
    SimpleDock dock;
    dock.show();
    dock.moveToEdge("Bottom");
    return app.exec();
}
