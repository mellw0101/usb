QT += core gui widgets svg dbus

TARGET = dock
CONFIG += c++17
CONFIG -= app_bundle
#CONFIG += link_pkgconfig
#PKGCONFIG += KF5WindowSystem

# main function
SOURCES += main.cpp

# SimpleDock functions
SOURCES += SimpleDock.cpp contextMenuEvent.cpp

# Delete updateDockGeometry.cpp createButtonLayout.cpp initializeButtons.cpp addButton.cpp savePid.cpp removeButton.cpp setButtonSize.cpp moveToEdge.cpp findIconForBinary.cpp findIconFilePath.cpp

# IconButton functions
SOURCES += IconButton.cpp

# debug
SOURCES += debug.cpp

# windowManager
SOURCES +=  windowManager.cpp \
            waylandClient.cpp \
            dbus.cpp \
            dbus_kwinScripting.cpp \
            dockConfig.cpp \
            highResAnimation.cpp \
            dPM.cpp

# Delete getPidByName.cpp appNameOfPid.cpp getCurrentProsseses.cpp removePidEntry.cpp

HEADERS +=  SimpleDock.h \
            IconButton.h \
            AppAddDialog.h \
            PidManager.h \
            debug.h \
            windowManager.h \
            waylandClient.h \
            dbus.h \
            dockConfig.h \
            highResAnimation.h \
            /usb/MLib/mlib.hpp \
            dPM.hpp \
            DockManager.hpp \
            toQString.hpp

INCLUDEPATH += /usr/include/KF5 /usr/include/KF5/KWindowSystem /usr/include/KF5/KWindowInfo /usr/include/KF5/KWayland /usb/MLib

# Library path - where the library files are located
LIBS += -L/usr/lib -L/usb/MLib

# Link against the KWindowSystem library
LIBS += -lKF5WindowSystem

LIBS += -L/usr/lib -lQt6Widgets -lQt6Gui -lQt6Core -lQt6DBus -lwayland-client
