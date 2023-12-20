#ifndef WCOMPOSITOR_HPP
#define WCOMPOSITOR_HPP

#include <QtWaylandCompositor/QWaylandCompositor>
#include <QtWaylandCompositor/QWaylandClient>

class WCompo : public QWaylandCompositor {
    Q_OBJECT

public:
    WCompo();
    void someFunction();
protected slots:
    void onNewClient(QWaylandClient *client);
    void signalRecived();

    // ... other methods and slots for client management ...

signals:
void requestMoveWindow(int x, int y);

private:
    // ... private members and methods ...
};

#endif // WCOMPOSITOR_H
