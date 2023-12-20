#include <QApplication>
#include <QMainWindow>
#include <QPainter>

class FullScreenBackground : public QMainWindow {
public:
    FullScreenBackground() {
        // Constructor: Set window flags and go full screen
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        showFullScreen();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        // Override paint event to draw the background
        QPainter painter(this);
        painter.fillRect(rect(), Qt::blue); // Fill the window with a blue color
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    FullScreenBackground window;
    window.show();

    return app.exec();
}
