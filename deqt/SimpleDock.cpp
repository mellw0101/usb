#include "SimpleDock.h"
#include "IconButton.h"
#include "PidManager.h"
#include "dbus_kwinScripting.h"
#include "debug.h"
#include "dbus.h"
#include "dockConfig.h"
#include "DockManager.hpp"
// #include "mlib.hpp"



#include "kx11extras.h"
#include "netwm_def.h"
#include "qdebug.h"
#include "qdockwidget.h"
#include "qflags.h"
#include "qlogging.h"
#include "qnamespace.h"
#include "qobject.h"
#include "qpoint.h"
#include "qpropertyanimation.h"
#include "qtmetamacros.h"
#include "qwidget.h"
#include "qwindow.h"
#include <QProcess>
#include <QGuiApplication>
#include <cstddef>
#include <cstdio>
#include <signal.h>
#include <type_traits>
#include <vector>
#include <QFileInfo>
#include <QDir>
#include <kwindowinfo.h>
#include <KWindowSystem>
#include <QWidget>
#include <QDockWidget>
#include <QWidgetAction>
#include <QWidgetData>
#include <QWidgetSet>
#include <QEasingCurve>
#include <QDockWidget>

SimpleDock::SimpleDock(): settings("Mellw", "dock") {
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus | Qt::WindowTitleHint);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setContentsMargins(0, 0, 0, 0);
    qDebug() << sizePolicy() << sizeHint();

    QObject::connect(&matcher, &DbusMatcher::matchResult, this, &SimpleDock::DBusMatchReceived);
    QObject::connect(&getWindowInfo, &DbusGetWindowInfo::getWindowInfoResult, this, &SimpleDock::DbusGetWindowInfoReceived);
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    initializeButtons();
}
void SimpleDock::updateDockGeometry() {                     // Update dock When Changes happen
    changeNum(TOTAL_WIDTH, num(BUTTON_SIZE) * buttons.count());
    mainLayout->setContentsMargins(0, 0, num(SQUEEZE_NUM), 0);
    debug(F, __func__, ": ", V, "totalWidth:", N, num(TOTAL_WIDTH), V, " BUTTON_SIZE:", N, num(BUTTON_SIZE), V, " ICON_SIZE:", N, num(ICON_SIZE), V, " SQUEEZE_NUM:", N, num(SQUEEZE_NUM), R);

    for (IconButton *button : buttons) {
        button->setIconSize(Qnum(ICON_SIZE));
        button->setFixedSize(Qnum(BUTTON_SIZE));
        QObject::connect(button, &IconButton::sizeChanged, this, &SimpleDock::sizeChangedRecived);
    }
    int height = ((num(BUTTON_SIZE) * num(SCALE_FACTOR)) / 100) + 2;
    setFixedHeight(height);
    updateGeometry();
    moveToEdge(current_placement);                          // Realign dock to correct placement
}
void SimpleDock::initializeButtons() {                      // Adds Spacers to dock
    QSpacerItem *frontSpacer = new QSpacerItem(10, num(BUTTON_SIZE), QSizePolicy::Expanding, QSizePolicy::Preferred);
    mainLayout->addSpacerItem(frontSpacer);
    restoreButtonsFromFile();
    mainLayout->addSpacerItem(backSpacer);
    updateDockGeometry();
}

void SimpleDock::addButton(QHBoxLayout *layout, const QString &app) {
    auto button = new IconButton(app, app, this);
    QString iconName = findIconForBinary(app);
    QIcon icon;

    QString iconPath = findIconFilePath(app);
    if (!iconName.isEmpty()) {
        if (QFile::exists(iconPath)) {
            debug(F, "addButton", R, ": Icon file for", V, " app", R, ": \"", app, "\" exists iconPath: ", iconPath);
            icon.addFile(iconPath);
            if (iconPath.endsWith(".png"))
                button->setIcon(makePngScaleble(iconPath));
            else
                button->setIcon(icon);
        } else {
            debug(F, "addButton", R, ": Icon file for", V, " app", R, ": \"", app, "\" Does not exist using Theme Icon: ", iconName);
            icon = QIcon::fromTheme(iconName);
            button->setIcon(icon);
        }
    }

    // Styling the button to look more like an icon
    button->setStyleSheet(
        "QPushButton { margin: 0px; padding: 0px; }"
        "QPushButton {"
        "border: none;" // removes the border
        "background-color: transparent;" // makes the background transparent
        "}"
        "QPushButton:hover {"
        "background-color: rgba(255, 255, 255, 0);" // slightly white background on hover
        "}"
        "QPushButton:pressed {"
        "background-color: rgba(255, 255, 255, 0);" // more white background when pressed
        "}"
    );
    connect(button, &IconButton::clicked, this, [this, app, button]() {
        /*QUuid uuid = */
        (dPM::process(app));
        button->bounceIcon();
        // dockManager.addToUuidMap(app, uuid);
        // dockManager.addToMap(uuidMap, app, uuid);
        // debug(uuidMap);
        // manager.updateDockManager(dockManager, button->appPath());
        // QProcess *process = new QProcess(this); // Create a new QProcess object as a child of the dock
        // QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        // env.insert("QT_QPA_PLATFORM", "wayland"); // Use Wayland
        // process->setProcessEnvironment(env);
        // process->start(app, QStringList()); // Start the process
        // button->bounceIcon();
    });

    buttons.append(button);
    layout->addWidget(button);
}
void SimpleDock::removeButton(IconButton *button) {         // Removes requsted button from dock
    if (buttons.removeOne(button)) { // Use removeOne, which returns true if an item was removed
        mainLayout->removeWidget(button); // Make sure you remove it from the correct layout
        button->deleteLater(); // Schedule the button for deletion
    }
    updateDockGeometry();
    saveButtonsToFile();
}
void SimpleDock::moveToEdge(const QString &edge) {          // Calculates and moves dock to correct pos
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    if (!screenGeometryShown) {
        qDebug() << "\e[35mmoveToEdge\e[0m: Is top-level:" << isWindow();
        qDebug() << "\e[35mmoveToEdge\e[0m: \e[34mscreenGeometry\e[0m:\e[32m" << screenGeometry << "\e[0m";
        screenGeometryShown = true;
    }
    int x, y;
    if (edge == "Top") {
        current_placement = "Top";
        x = ((screenGeometry.width() - width()) / 2);
        y = 0;
    } else if (edge == "Bottom") {
        current_placement = "Bottom";
        x = ((screenGeometry.width() - width()) / 2);
        y = screenGeometry.height() - height();
    } else if (edge == "Left") {
        current_placement = "Left";
        x = 0;
        y = (screenGeometry.height() - height()) / 2;
    } else if (edge == "Right") {
        current_placement = "Right";
        x = screenGeometry.width() - width();
        y = (screenGeometry.height() - height()) / 2;
    } else {    // Default to Bottom if edge is not recognized
        x = (screenGeometry.width() - width()) / 2;
        y = screenGeometry.height() - height();
    }
    qDebug() << "\e[35mmoveToEdge\e[0m:\e[32m \e[34mx:\e[33m" << x << "\e[34my:\e[33m" << y << "\e[0m";
    move(x, y);
}
QString SimpleDock::findIconForBinary(const QString& binaryPath) {
    QString binary = QFileInfo(binaryPath).fileName();

    QDir applicationsDir("/usr/share/applications/");
    QStringList desktopFiles = applicationsDir.entryList(QStringList() << "*" + binary + "*.desktop", QDir::Files);

    for (const QString& desktopFileName : desktopFiles) {
        QFile file(applicationsDir.filePath(desktopFileName));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString line;
            QString iconName;

            while (!in.atEnd()) {
                line = in.readLine();
                if (line.startsWith("Exec=") && line.contains(binary, Qt::CaseInsensitive)) {
                    while (!in.atEnd()) {
                        line = in.readLine();
                        if (line.startsWith("Icon=")) {
                            iconName = line.section('=', 1, 1).trimmed();
                            return iconName;  // Return the icon name to use it to find the path
                        }
                    }
                }
            }
            file.close();
        }
    }

    // Fallback icon name if nothing is found
    return "application-default-icon";
}
QString SimpleDock::findIconFilePath(const QString& iconName) {
    QStringList iconSizes = {"256x256", "128x128", "64x64", "48x48", "32x32"};
    QStringList imageExtensions = {"png", "svg", "xpm"};
    QString shortIconName = iconName.section("/", -1);

    QString scalableDir = "/usr/share/icons/hicolor/scalable/apps/";
    QString sizeDirBase = "/usr/share/icons/hicolor/";
    QString breezeDarkDirBase = "/usr/share/icons/breeze-dark/actions/24/";

    auto checkExistsWildcard = [](const QString& dir, const QString& namePart, const QStringList& extensions) -> QString {
        QDir directory(dir);
        for (const QString& ext : extensions) {
            QStringList filter;
            filter << "*" + namePart + "*." + ext; // Search for any filename containing the name part
            directory.setNameFilters(filter);
            QStringList entries = directory.entryList(filter, QDir::Files);
            if (!entries.isEmpty()) {
                return dir + entries.first(); // Return the first match
            }
        }
        return QString();
    };

    QString filePath = checkExistsWildcard(scalableDir, shortIconName, imageExtensions);
    if (!filePath.isEmpty()) return filePath;

    for (const QString& size : iconSizes) {
        filePath = checkExistsWildcard(sizeDirBase + size + "/apps/", shortIconName, imageExtensions);
        if (!filePath.isEmpty()) return filePath;
    }

    filePath = checkExistsWildcard(breezeDarkDirBase, shortIconName, imageExtensions);
    if (!filePath.isEmpty()) return filePath;

    // Split and search each part if no icon found
    QStringList nameParts = shortIconName.split('-');
    for (const QString& part : nameParts) {
        filePath = checkExistsWildcard(scalableDir, part, imageExtensions);
        if (!filePath.isEmpty()) return filePath;

        for (const QString& size : iconSizes) {
            filePath = checkExistsWildcard(sizeDirBase + size + "/apps/", part, imageExtensions);
            if (!filePath.isEmpty()) return filePath;
        }

        filePath = checkExistsWildcard(breezeDarkDirBase, part, imageExtensions);
        if (!filePath.isEmpty()) return filePath;
    }

    // No icon found
    return QString();
}
void SimpleDock::openAppAddDialog() {                       // Open dialog window to search for apps
    AppAddDialog *dialog = new AppAddDialog(this);
    connect(dialog, &QDialog::accepted, [this, dialog]() {  // Handle the accepted signal to add the app
        QString selectedAppPath = dialog->appPath;
        mainLayout->removeItem(backSpacer);
        addButton(mainLayout, selectedAppPath);
        mainLayout->addSpacerItem(backSpacer);
        saveButtonsToFile();
        updateDockGeometry();
    });

    dialog->exec();
}
QPixmap SimpleDock::makePngScaleble(const QString& path) {  // Turn PNG into QPixmap
    QPixmap originalPixmap(path);
    QPixmap scaledPixmap = originalPixmap.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    return scaledPixmap;
}
void SimpleDock::saveButtonsToFile() {                      // Saves current app layout to FILE in order
    std::ofstream file(dockFile(APP_ORDER));
    if (!file.is_open()){
        DEBUG::output("saveButtonsToFile: could not open file:");
        return;
    }
    QStringList names;
    for (IconButton *button : buttons) {
        QString name = button->appPath() + ":" + QString::number(mainLayout->indexOf(button));
        names << name;
        qDebug() << names;
    }
    int placeToFind = 1;
    int lastPlace = mainLayout->count() - 3;
    while (placeToFind <= lastPlace) {
        for (const QString &name : names) {
            QStringList parts = name.split(":");
            qDebug() << parts;
            if (parts[1] == QString::number(placeToFind)) {
                file << parts[0].toStdString() << "\n";
                placeToFind += 1;
            }
        }
    }
    file.close();
}
void SimpleDock::restoreButtonsFromFile() {                 // Restores app order from FILE
    QStringList order;
    std::ifstream file(dockFile(APP_ORDER));

    if (!file.is_open()) {
        DEBUG::output("restoreButtonsFromFile: could not open file: .dockAppOrder");
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        order << QString::fromStdString(line);
    }
    file.close();

    DEBUG::output(DEBUG::QStringList_to_QString(order));

    if (order.empty())
        addButton(mainLayout, "konsole");

    int numberOfButtons = 0;
    for (auto app : order) {
        addButton(mainLayout, app);
        numberOfButtons += 1;
    }

}
void SimpleDock::moveButton(IconButton* button, MoveDirection direction) { // Move Button
    int index = mainLayout->indexOf(button);
    int newIndex = (direction == MoveDirection::Left) ? index - 1 : index + 1;

    // Check if new index is within the valid range
    if (newIndex >= 0 && newIndex < mainLayout->count()) {
        mainLayout->removeWidget(button);
        mainLayout->insertWidget(newIndex, button);
        mainLayout->update();
    }
    saveButtonsToFile();
}
void SimpleDock::killJob(const int &pid) {                  // Kills requsted PID
    debug(F, "killJob", R, ": ", V, "PID", R, ": ", N, pid, R);
    kill(pid, SIGTERM);
}
void SimpleDock::checkIfWayland() {
    const char* wayland = getenv("QT_QPA_PLATFORM");
    if (wayland != NULL) {
        session = "x11";
        return;
    }
    session = "wayland";
}
