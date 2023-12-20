#include <QApplication>
#include <QHBoxLayout>
#include <QPushButton>
#include <QtWidgets>
#include <QtGui>
#include <QProcess>

// void addButton(const QString &name, const QString &app, QHBoxLayout *layout) {
//     auto button = new QPushButton(name);
//     connect(button, &QPushButton::clicked, [app]() {
//         QProcess::startDetached(app, QStringList()); // Starts the application detached
//     });
//     layout->addWidget(button); // Add the button to the passed layout
// }
// #include <QApplication>
// #include <QHBoxLayout>
// #include <QPushButton>
// #include <QtWidgets>
// #include <QtGui>
// #include <QProcess>
// #include <QMenu>
// #include <QScreen>
// #include <QGuiApplication>
//
// class SimpleDock : public QWidget {
// public:
//     SimpleDock() {
//         setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
//         setAttribute(Qt::WA_TranslucentBackground);
//
//         auto layout = new QHBoxLayout(this); // Create a layout for this widget
//         layout->setSpacing(0);
//         layout->setContentsMargins(0, 0, 0, 0);
//
//         // Example: Add a few application launch buttons
//         addButton("Term", "/usr/bin/konsole", layout);
//         addButton("FileM", "/usr/bin/dolphin", layout);
//         addButton("Browser", "/usr/bin/falkon", layout);
//     }
//     // Copy and move constructors and assignment operators as default
//     SimpleDock(const SimpleDock &) = delete;
//     SimpleDock(SimpleDock &&) = delete;
//     SimpleDock &operator=(const SimpleDock &) = delete;
//     SimpleDock &operator=(SimpleDock &&) = delete;
// protected:
//     void contextMenuEvent(QContextMenuEvent *event) override {
//         QMenu contextMenu(this);
//
//         QAction actionTop("Top", this);
//         connect(&actionTop, &QAction::triggered, this, [this] { moveToEdge("Top"); });
//         contextMenu.addAction(&actionTop);
//
//         QAction actionBottom("Bottom", this);
//         connect(&actionBottom, &QAction::triggered, this, [this] { moveToEdge("Bottom"); });
//         contextMenu.addAction(&actionBottom);
//
//         QAction actionLeft("Left", this);
//         connect(&actionLeft, &QAction::triggered, this, [this] { moveToEdge("Left"); });
//         contextMenu.addAction(&actionLeft);
//
//         QAction actionRight("Right", this);
//         connect(&actionRight, &QAction::triggered, this, [this] { moveToEdge("Right"); });
//         contextMenu.addAction(&actionRight);
//
//         QMenu *sizeMenu = new QMenu("Size", this);
//
//         // Create actions for different sizes
//         QAction *size40 = sizeMenu->addAction("40", this, [this]{ setButtonSize(40); });
//         QAction *size42 = sizeMenu->addAction("42", this, [this]{ setButtonSize(42); });
//         QAction *size44 = sizeMenu->addAction("44", this, [this]{ setButtonSize(44); });
//         QAction *size46 = sizeMenu->addAction("46", this, [this]{ setButtonSize(46); });
//         QAction *size48 = sizeMenu->addAction("48", this, [this]{ setButtonSize(48); });
//         QAction *size50 = sizeMenu->addAction("50", this, [this]{ setButtonSize(50); });
//         QAction *size52 = sizeMenu->addAction("52", this, [this]{ setButtonSize(52); });
//         QAction *size54 = sizeMenu->addAction("54", this, [this]{ setButtonSize(54); });
//         QAction *size56 = sizeMenu->addAction("56", this, [this]{ setButtonSize(56); });
//         QAction *size58 = sizeMenu->addAction("58", this, [this]{ setButtonSize(58); });
//
//         // Add the submenu to the main context menu
//         contextMenu.addMenu(sizeMenu);
//
//         // Execute the context menu
//         contextMenu.exec(event->globalPos());
//     }
// private:
//     QHBoxLayout *layout;  // Now a member variable
//     QList<QPushButton*> buttons;  // Keep track of buttons
//     void setButtonSize(int edgeLength) {
//         // Iterate over the buttons and set their new size
//         for (QPushButton *button : buttons) {
//             button->setFixedSize(edgeLength, edgeLength);
//         }
//     }
//     void moveToEdge(const QString &edge) {
//         QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
//         int x, y;
//
//         if (edge == "Top") {
//             x = (screenGeometry.width() - width()) / 2;
//             y = 10;
//         } else if (edge == "Bottom") {
//             x = (screenGeometry.width() - width()) / 2;
//             y = screenGeometry.height() - height() - 10;
//         } else if (edge == "Left") {
//             x = 10;
//             y = (screenGeometry.height() - height()) / 2;
//         } else if (edge == "Right") {
//             x = screenGeometry.width() - width() - 10;
//             y = (screenGeometry.height() - height()) / 2;
//         } else {    // Default to Bottom if edge is not recognized
//             x = (screenGeometry.width() - width()) / 2;
//             y = screenGeometry.height() - height();
//         }
//         move(x, y);
//     }
//     void addButton(const QString &name, const QString &app, QHBoxLayout *layout) {
//         auto button = new QPushButton(name);
//         connect(button, &QPushButton::clicked, [app]() {
//             QProcess::startDetached(app, QStringList());
//         });
//         int edgeLength = 52;  // Default size for the button to make it square.
//         button->setFixedSize(edgeLength, edgeLength);
//         buttons.append(button);  // Add the button to the list
//         layout->addWidget(button);
//     }
// };
//
// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);
//     SimpleDock dock;
//     dock.show();
//     return app.exec();
// }

// void addButton(const QString &name, const QString &app) {
//         auto button = new QPushButton(name);
//         connect(button, &QPushButton::clicked, [app]() {
//             QProcess::startDetached(app, QStringList());
//         });
//         button->setFixedSize(70, 70); // Default button size
//         buttons.append(button);
//         layout->addWidget(button);
//     }

class AppAddDialog : public QDialog {
public:
    QString appPath;
    AppAddDialog(QWidget *parent = nullptr) : QDialog(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        QLineEdit *searchEdit = new QLineEdit(this);
        QListWidget *listWidget = new QListWidget(this);
        QPushButton *addButton = new QPushButton("Add", this);
        QPushButton *cancelButton = new QPushButton("Cancel", this);

        layout->addWidget(searchEdit);
        layout->addWidget(listWidget);
        layout->addWidget(addButton);
        layout->addWidget(cancelButton);

        connect(searchEdit, &QLineEdit::textChanged, this, [listWidget](const QString &searchTerm) {
            listWidget->clear();
            QDirIterator it("/usr/bin/", QStringList(), QDir::Files);
            // QDirIterator it("/usr/share/applications/", QStringList() << "*.desktop", QDir::Files);
            while (it.hasNext()) {
                QString filename = it.next();
                if (filename.contains(searchTerm, Qt::CaseInsensitive)) {
                    listWidget->addItem(filename.section("/", -1));
                }
            }
        });

        connect(addButton, &QPushButton::clicked, this, [this, listWidget]() {
            QList<QListWidgetItem *> selectedItems = listWidget->selectedItems();
            if (!selectedItems.isEmpty()) {
                appPath = selectedItems.first()->text();
                // Emit a signal or perform an action to add the app
                accept();
            }
        });

        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

        setWindowTitle("Add New App");
    }
};
QString dockWidgetPosition(QMainWindow *mainWindow, QDockWidget *dockWidget) {
    if (!mainWindow || !dockWidget) {
        return "Invalid pointers";
    }

    // Get the geometry of the main window and the dock widget
    QRect mainWindowRect = mainWindow->geometry();
    QRect dockWidgetRect = dockWidget->geometry();

    // Calculate the center points
    QPoint mainWindowCenter = mainWindowRect.center();
    QPoint dockWidgetCenter = dockWidgetRect.center();

    // Check positions relative to center points
    if (dockWidgetCenter.y() < mainWindowCenter.y()) {
        // Dock widget is in the upper half of the main window
        return "Top";
    } else if (dockWidgetCenter.y() > mainWindowCenter.y()) {
        // Dock widget is in the lower half of the main window
        return "Bottom";
    } else if (dockWidgetCenter.x() < mainWindowCenter.x()) {
        // Dock widget is in the left half of the main window
        return "Left";
    } else if (dockWidgetCenter.x() > mainWindowCenter.x()) {
        // Dock widget is in the right half of the main window
        return "Right";
    }

    return "Center";
}
// if (!icon.isNull() && !icon.availableSizes().isEmpty()) {
        //     qDebug() << "Setting icon with available sizes";
        //     button->setIcon(icon);
        //     button->setIconSize(QSize(current_button_size - 12, current_button_size - 12));
        // } else if (iconPath != "") {
        //     button->setIcon(icon);
        // } else {
        //     qDebug() << "Icon is null or no sizes available, setting fallback text";
        //     button->setText("n");
        // }


// void setWindowToLaunch(QWidget *window) {
//         windowToLaunch = window;
//         window->hide(); // Initially hide the window
//     }
//     // Allow setting a factory function that creates the window
//     void setWindowFactory(std::function<QWidget*()> factory) {
//         windowFactory = std::move(factory);
//     }
//     void launchWindowAnimation() {
//         if (!windowToLaunch)
//             return;
//
//         // Get the icon's global position
//         QPoint iconGlobalPos = mapToGlobal(rect().center());
//
//         // Set the window's initial geometry
//         windowToLaunch->setGeometry(QRect(iconGlobalPos, QSize(1, 1))); // Start with a tiny window
//
//         // Create a size animation
//         QPropertyAnimation *sizeAnim = new QPropertyAnimation(windowToLaunch, "geometry");
//         sizeAnim->setDuration(500); // Animation duration
//
//         QRect finalGeometry = windowToLaunch->geometry();
//         finalGeometry.moveCenter(iconGlobalPos); // Center it on the icon initially
//
//         sizeAnim->setStartValue(QRect(iconGlobalPos, QSize(1, 1)));
//         sizeAnim->setEndValue(finalGeometry);
//
//         QObject::connect(sizeAnim, &QPropertyAnimation::finished, windowToLaunch, &QWidget::show);
//
//         sizeAnim->start(QAbstractAnimation::DeleteWhenStopped);
//     }

// void mousePressEvent(QMouseEvent *event) override {
//         QPushButton::mousePressEvent(event);
//         if (event->button() == Qt::LeftButton) {
//             // Save the start position
//             m_dragStartPosition = event->pos();
//         }
//     }
//     void mouseMoveEvent(QMouseEvent *event) override {
//         if (!(event->buttons() & Qt::LeftButton)) return;
//         if ((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance()) return;
//
//         QDrag *drag = new QDrag(this);
//         QMimeData *mimeData = new QMimeData;
//
//         // Set the data to be dragged; here, we're using the button's index
//         mimeData->setData("application/x-iconbutton-index", QByteArray::number(buttonIndex));
//         drag->setMimeData(mimeData);
//
//         // Execute the drag operation
//         drag->exec(Qt::MoveAction);
//     }
/*
void reorderButtons(int originalIndex, int newIndex) {
        // Assume layout is a pointer to the QHBoxLayout or QVBoxLayout with the buttons
        QHBoxLayout* layout = this->buttonLayout;

        if (layout != nullptr && originalIndex >= 0 && newIndex >= 0 && originalIndex < layout->count() && newIndex < layout->count()) {
            // Take the widget out of the layout
            QWidget* widgetToMove = layout->itemAt(originalIndex)->widget();

            if (widgetToMove != nullptr) {
                layout->removeWidget(widgetToMove);

                // Insert the widget at the new index
                layout->insertWidget(newIndex, widgetToMove);

                // Optional: Update indices for all buttons, if they are storing their own index
                for (int i = 0; i < layout->count(); ++i) {
                    IconButton* button = qobject_cast<IconButton*>(layout->itemAt(i)->widget());
                    if (button) {
                        button->buttonIndex = i;
                    }
                }

                // If you are maintaining any list or structure to keep track of buttons, make sure to update that as well
                // ...

                // Update the layout
                layout->update();
            }
        }
    }*/
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
// class TrashCanIcon : public QPushButton {
// public:
//     TrashCanIcon(QWidget *parent = nullptr) : QPushButton(parent) {
//         setAcceptDrops(true);  // Allow the widget to accept dropped items
//         // Set the button's icon to a trash can image
//         setIcon(QIcon("/usr/share/icons/Adwaita/scalable/places/user-trash.svg"));
//         setIconSize(QSize(48, 48));  // Or any other size
//     }
// protected:
//     void dragEnterEvent(QDragEnterEvent *event) override {// Allow drag enter when it's valid
//         if (event->mimeData()->hasFormat("application/x-item")) {
//             event->setDropAction(Qt::MoveAction);
//             event->accept();
//         } else {
//             event->ignore();
//         }
//     }
//     void dragMoveEvent(QDragMoveEvent *event) override {// Allow drag move when it's valid
//         if (event->mimeData()->hasFormat("application/x-item")) {
//             event->setDropAction(Qt::MoveAction);
//             event->accept();
//         } else {
//             event->ignore();
//         }
//     }
//     void dropEvent(QDropEvent *event) override {// Handle the drop event to perform the trash action
//         if (event->mimeData()->hasFormat("application/x-item")) {
//             // Perform the action to delete or move the item to the trash
//             // ...
//
//             event->setDropAction(Qt::MoveAction);
//             event->accept();
//         } else {
//             event->ignore();
//         }
//     }
// };
