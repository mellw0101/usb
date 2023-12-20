#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "/usr/include/qt6/QtWidgets/qmainwindow.h"
#include "/usr/include/qt6/QtWidgets/qwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent)
        : QMainWindow(parent) {
        // Set the title of the window
        // setWindowTitle("Qt Window");

        // Set the initial size of the window
        resize(800, 600);
        // WindowManager this;
        // Additional setup like creating menus, toolbars, etc., goes here.
    }
    ~MainWindow();

public slots:
    void moveTo(int x, int y);

    // ...
};
#endif // MAINWINDOW_HPP



