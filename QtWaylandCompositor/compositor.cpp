#include <QGuiApplication>
#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandQuickCompositor>
#include <QtWaylandCompositor/QWaylandQuickShellSurfaceItem>
#include <QtWaylandCompositor/QWaylandSurface>
#include <QQuickView>
#include <QtWaylandCompositor/QWaylandQuickCompositor>
#include <QtWaylandCompositor/QWaylandOutput>
#include <QtWaylandCompositor/QWaylandXdgShell>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QWaylandQuickCompositor compositor;

    // Create a Wayland output for the view
    QWaylandOutput *output = new QWaylandOutput(&compositor, &view);
    compositor.setDefaultOutput(output);

    // Create an Xdg Shell
    QWaylandXdgShell xdgShell(&compositor);
    compositor.addExtension(&xdgShell);

    // Initialize and show the view
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    return app.exec();
}
