#include "mainWindow.hpp"
#include <QMainWindow>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // Set the title of the window
    // setWindowTitle("Qt Window");

    // Set the initial size of the window
    resize(800, 600);
    // WindowManager this;
    // Additional setup like creating menus, toolbars, etc., goes here.
}

MainWindow::~MainWindow() {
    // Cleanup code if necessary
}
void MainWindow::moveTo(int x, int y) {
    this->move(x, y);
}

