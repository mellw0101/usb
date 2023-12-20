QT += core gui widgets svg

TARGET = dock
CONFIG += c++17
CONFIG -= app_bundle

# Input
SOURCES += dock.cpp
#HEADERS += dock.cpp  # This line is unconventional but it forces moc to run on the .cpp file.

INCLUDEPATH += /usr/include/qt6
INCLUDEPATH += /usr/include/qt6/QtCore
INCLUDEPATH += /usr/include/qt6/QtGui
INCLUDEPATH += /usr/include/qt6/QtWidgets

LIBS += -L/usr/lib -lQt6Widgets -lQt6Gui -lQt6Core
