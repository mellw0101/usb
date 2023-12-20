#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "SimpleDock.h"
#include <QDBusReply>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QVariant>
#include <QDebug>
#include <QStringList>

class WindowManager : public QObject {
    Q_OBJECT

    int desktop;
    SimpleDock dock;
public:
    WindowManager();
    int currentDesktop();
private slots:
    void getCurrentDesktop();
};

#endif // WINDOWMANAGER_H
