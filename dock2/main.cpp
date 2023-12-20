#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandClient>
#include <QApplication>
#include <QGuiApplication>
#include <QWindow>
#include <iostream>
#include "mlib.hpp"

#include <QApplication>
#include <QString>
#include <QGuiApplication>
#include <QMainWindow>
#include "QtWaylandCompositor/qwaylandclient.h"
#include <QtWaylandCompositor/QWaylandCompositorExtension>
#include "QtWaylandCompositor/qwaylandoutputmode.h"
#include "mainWindow.hpp"
#include "qguiapplication.h"
// #include "QtGui/qguiapplication_platform.h"
#include "qmainwindow.h"
#include <QtWaylandCompositor/QWaylandClient>
#include "qwidget.h"
#include "qwindowdefs.h"
#include "wCompositor.hpp"
#include <wayland-client.h>
#include "/usb/MLib/waylandClient.hpp"

//int argc, char **argv


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // setenv("QT_QPA_PLATFORM", "xcb", 1);
    MainWindow window;
    window.show();

    WaylandClient client;
    client.run();
    client.create_surface();
    return app.exec();
}
// int main(int argc, char *argv[]) {
//     QCoreApplication app(argc, argv);
//
//     AdvancedCompositor compositor;
//     // Additional setup for the compositor if needed
//
//     return app.exec();
// }
