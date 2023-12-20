#include "qlogging.h"
#include "qwidget.h"
#include "qwindow.h"
#include "qwindowdefs.h"
#include <QGuiApplication>
#include <QWindow>
#include <QMouseEvent>
#include <QScreen>
#include <QImage>
#include <QPainter>
#include <QBackingStore>
#include <QDebug>
#include <QThread>
#include <QFile>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QBackingStore>
#include <QDebug>
#include <QProcess>


#include <linux/input.h>  // For input_event structure

class Window : public QWidget {
public:
    Window(QWidget *parent = nullptr) : QWidget(parent) {
        // Set window properties, like size, title, etc.
    }

    // Override paintEvent to draw window contents.
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        // ... drawing code ...
    }

    // Add more event handlers as needed (mouse, keyboard, etc.)
};

class WindowManager : public QObject {
    Q_OBJECT
private:
    QList<Window*> windows;

public:
    WindowManager() {
        // Initialize the window manager
    }

    void addWindow(Window* window) {
        windows.append(window);
        // Perform additional setup as necessary
    }

    // Add methods to manage window focus, z-order, etc.
};

class de : public QWidget {
public:
    de() {
        setWindowFlags(windowFlags() | Qt::WindowStaysOnBottomHint);
        setAttribute(Qt::WA_NoSystemBackground, true);
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        setGeometry(QApplication::primaryScreen()->geometry());
        showFullScreen();

        QProcess *process = new QProcess(this);
        process->start("konsole");
    }

    void onTestButtonClick() {
        qDebug() << "Button clicked!";
    }

    // Override paintEvent instead of using QBackingStore
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        QImage backgroundImage("/home/mellw/Pictures/1.png");
        if (!backgroundImage.isNull()) {
            painter.drawImage(0, 0, backgroundImage.scaled(size()));
        }
    }
};


class MouseReader : public QThread {
    QWidget *targetWidget;  // Pointer to the widget to receive the events
    // QWindow *targetWindow;  // Pointer to the window to receive the events
    int newX = 0;  // Current x position
    int newY = 0;  // Current y position

public:
    explicit MouseReader(QWidget *widget) : QThread(nullptr), targetWidget(widget) {

    }

    void run() override {
        qDebug() << "run ";

        QFile inputFile("/dev/input/mouse0");               // Replace with your actual event file
        if (inputFile.open(QIODevice::ReadOnly)) {
            // Buffer to store the read data
            char buffer[sizeof(struct input_event)];
            struct input_event* event;

            while (inputFile.isOpen() && !inputFile.atEnd()) {
                // Reading input_event from device
                inputFile.read(buffer, sizeof(struct input_event));
                event = reinterpret_cast<struct input_event*>(buffer);

                // Now, depending on the type of event, create a QMouseEvent
                // This is a very simplified example
                if (event->type == EV_REL && event->code == REL_X) {
                    newX += event->value; // Update the x position
                    // Calculate the new x position based on the event->value
                } else if (event->type == EV_REL && event->code == REL_Y) {
                    newY += event->value; // Update the y position// This is a relative X movement
                    // This is a relative Y movement
                    // Calculate the new y position based on the event->value
                } else if (event->type == EV_KEY && (event->code == BTN_LEFT || event->code == BTN_RIGHT)) {
                    // This is a button press or release
                    Qt::MouseButton button;
                    if (event->code == BTN_LEFT) {
                        button = Qt::LeftButton;
                    } else {
                        button = Qt::RightButton;
                    }

                    QMouseEvent *mouseEvent = new QMouseEvent(
                        event->value == 1 ? QEvent::MouseButtonPress : QEvent::MouseButtonRelease,
                        QPointF(newX, newY), // Use QPointF for floating point precision
                        QPointF(newX, newY), // screenPos - in most cases will be the same as localPos
                        button,
                        button,
                        Qt::NoModifier
                    );

                    // Post the event to the target window
                    QCoreApplication::postEvent(targetWidget, mouseEvent);
                    qDebug() << newX << newY;
                }
                // Add more cases as necessary for different types of mouse movements or button presses

                // Sleep for a short period of time to prevent hogging the CPU
                this->msleep(10);
            }
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    de window;  // 'de' is a subclass of 'QWindow'
    window.show();  // Make sure the window is visible

    // Pass the address of 'window' to the MouseReader constructor
    MouseReader mouseReader(&window);

    // Start the MouseReader thread
    mouseReader.start();

    // Enter the main event loop
    return app.exec();
}
