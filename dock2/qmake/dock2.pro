# QT += core gui widgets waylandclient
#
# greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#
# CONFIG += c++17
# TARGET = dock
# TEMPLATE = app
#
# # QMAKE_CXX = clang++
# # QMAKE_CXXFLAGS -= -Wall
# # QMAKE_CFLAGS -= -Wall
#
# SOURCES += \
#         ../main.cpp
#
# HEADERS += \

QT += core gui widgets svg dbus waylandclient waylandcompositor

TARGET = dock
CONFIG += c++17
CONFIG -= app_bundle
#CONFIG += link_pkgconfig
#PKGCONFIG += KF5WindowSystem

# main function
SOURCES += \
        ../main.cpp \
        ../mainWindow.cpp \
        ../wCompositor.cpp

HEADERS += \
        ../mainWindow.hpp \
        ../wCompositor.hpp \
        /usb/MLib/mlib.hpp \
        /usb/MLib/waylandClient.hpp

INCLUDEPATH += /usr/include/KF5 /usr/include/KF5/KWindowSystem /usr/include/KF5/KWindowInfo /usr/include/KF5/KWayland /usb/MLib

# Library path - where the library files are located
LIBS += -L/usr/lib -L/usb/MLib

# Link against the KWindowSystem library
LIBS += -lKF5WindowSystem

LIBS += -L/usr/lib -lQt6Widgets -lQt6Gui -lQt6Core -lQt6DBus -lwayland-client -lwayland-server
