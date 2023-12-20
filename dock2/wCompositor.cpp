#include "wCompositor.hpp"
#include "QtWaylandCompositor/qwaylandcompositor.h"

WCompo::WCompo() {
    // Initialization of the compositor

    someFunction();
}

void WCompo::signalRecived(){

}
void WCompo::onNewClient(QWaylandClient *client) {
    qDebug() << "got responce";
    // Handle the new client
    // 'client' is a pointer to the connected QWaylandClient

    // You can store the client in a list, set up resources, etc.
}
void WCompo::someFunction() {
    int i = 0;
    while (i <= 5){
        emit requestMoveWindow(300, 300); // Example coordinates
        i += 1;
    }
}
