#include "qtclasshelpermacros.h"
#include "qtmetamacros.h"
#include <QApplication>
#include <QContextMenuEvent>
#include <QDialog>
#include <QDir>
#include <QDirIterator>
#include <QDockWidget>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLineEdit>
#include <QList>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMimeData>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QProcess>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QScreen>
#include <QSequentialAnimationGroup>
#include <QSettings>
#include <QSize>
#include <QStringList>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWidget>
#include <cstdlib>
#include <string>

class TrashCanIcon : public QPushButton {
public:
    TrashCanIcon(QWidget *parent = nullptr) : QPushButton(parent) {
        setAcceptDrops(true);  // Allow the widget to accept dropped items
        // Set the button's icon to a trash can image
        setIcon(QIcon("/usr/share/icons/Adwaita/scalable/places/user-trash.svg"));
        setIconSize(QSize(48, 48));  // Or any other size
    }
protected:
    void dragEnterEvent(QDragEnterEvent *event) override {// Allow drag enter when it's valid
        if (event->mimeData()->hasFormat("application/x-item")) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->ignore();
        }
    }
    void dragMoveEvent(QDragMoveEvent *event) override {// Allow drag move when it's valid
        if (event->mimeData()->hasFormat("application/x-item")) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->ignore();
        }
    }
    void dropEvent(QDropEvent *event) override {// Handle the drop event to perform the trash action
        if (event->mimeData()->hasFormat("application/x-item")) {
            // Perform the action to delete or move the item to the trash
            // ...

            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->ignore();
        }
    }
};

class IconButton : public QPushButton {
    std::function<QWidget*()> windowFactory;
    QWidget *windowToLaunch = nullptr;
    QString name_; // This should already be a part of your IconButton class
    QString appPath_; // Private member to store the application path
public:
    explicit IconButton(const QString& name, const QString& appPath , QWidget *parent = nullptr)
        : QPushButton(parent), name_(name), appPath_(appPath), scaleFactor(1.2f), animDuration(40) {
            // Initialize icon sizes
            normalButtonSize = QSize(60, 60); // This sets the normal button size
            normalIconSize = QSize(48, 48); // Set this to your desired icon size

            setIconSize(normalIconSize); // Set the initial icon size
            setFixedSize(normalButtonSize); // Set the button's fixed size

            // Prepare the animation with the property "iconSize"
            animation = new QPropertyAnimation(this, "iconSize");
            animation->setDuration(animDuration);
        }
    ~IconButton() {
        delete animation;
    }
    void updateSize(const int num) {
        normalButtonSize = QSize(num, num);
        normalIconSize = QSize(num - 12, num - 12);
    }
    void updateScaleFactor(const float &num) {
        scaleFactor = num;
    }
    void updateAnimDuration(const int& num){
        animDuration = num;
    }
    QString appPath() const {                               // Implement the appPath() accessor method
        return appPath_;
    }
    QString getName() const {
        return name_;
    }
protected:
    void enterEvent(QEnterEvent *event) override {
        animateSize(normalIconSize * scaleFactor);
        QPushButton::enterEvent(event);
    }
    void leaveEvent(QEvent *event) override {
        animateSize(normalIconSize);
        QPushButton::leaveEvent(event);
    }
    void setAnimatedIconSize(const QSize &size) {
        QPushButton::setIconSize(size);
        // updateGeometry();
        setFixedSize(size); // Adjust this if using layouts
    }
    QSize iconSize() const {
        return QPushButton::iconSize();
    }
private:
    QSize normalIconSize;
    QSize normalButtonSize;
    float scaleFactor;
    QPropertyAnimation *animation;
    int animDuration;

    void animateSize(const QSize &newSize) {
        if(animation->state() == QAbstractAnimation::Running) {
            animation->stop();
        }
        animation->setStartValue(iconSize());
        animation->setEndValue(newSize);
        animation->start();
    }
};

class KeyboardEventFilter : public QObject {
protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::KeyPress) {
            // Check if the object is one of the dialog windows that require input
            QDialog* dialog = dynamic_cast<QDialog*>(obj);
            if (dialog && dialog->objectName() == "DialogWindow") {
                // Allow the event to be processed further for dialog windows
                return QObject::eventFilter(obj, event);
            } else {
                // For all other widgets, ignore the key press event
                return true;
            }
        }
        // For all other types of events, process them normally
        return QObject::eventFilter(obj, event);
    }
};

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
            QString USER = getenv("USER");
            listWidget->clear();
            QStringList directories = {
                "/usr/bin/",
                "/usr/share/applications/",
                "/home/" + USER + "/.local/share/applications"
            };

            for (const QString& dir : directories) {
                QDirIterator it(dir, QDir::Files);
                while (it.hasNext()) {
                    it.next();
                    QString filename = it.fileName();
                    if (filename.contains(searchTerm, Qt::CaseInsensitive)) {
                        // Make sure not to insert duplicates
                        QList<QListWidgetItem *> found = listWidget->findItems(filename.section("/", -1), Qt::MatchExactly);
                        if (found.empty()) {
                            listWidget->addItem(filename.section("/", -1));
                        }
                    }
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

class SimpleDock : public QWidget {
    // Add new members to handle settings
    QSettings settings;
private:
    QHBoxLayout *layout;
    QVBoxLayout *mainLayout; // Main layout for the widget
    QHBoxLayout *buttonLayout; // Layout for the buttons
    QList<IconButton*> buttons; // Use IconButton instead of QPushButton
    QPoint dragPosition; // to store the initial click position
public:
    int current_button_size = 60;
    QString current_placement = "Buttom";

    SimpleDock() : settings("Mellw", "dock"){
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);
        setAttribute(Qt::WA_TranslucentBackground);

        mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        buttonLayout = createButtonLayout(); // correct the type here
        initializeButtons(buttonLayout);

        mainLayout->addLayout(buttonLayout);

        setLayout(mainLayout);
    }
    void updateDockGeometry() {                             // New function to update dock geometry
        int totalWidth = 0;
        // Calculate the total width of all buttons including margins and spacing
        for (IconButton *button : buttons) {
            totalWidth += button->width();
        }
        // Add the stretch spacing if it's not the only element
        if (buttons.count() > 1) {
            totalWidth += buttonLayout->spacing() * (buttons.count() - 1);
        }
        setFixedWidth(totalWidth);                          // Set the dock width
        moveToEdge(current_placement);                      // Realign dock to correct placement
    }
    QHBoxLayout* createButtonLayout() {
        buttonLayout = new QHBoxLayout();
        buttonLayout->setContentsMargins(0, 0, 0, 0);
        buttonLayout->setSpacing(0);
        buttonLayout->setAlignment(Qt::AlignCenter);
        buttonLayout->addStretch(1);
        return buttonLayout;
    }
    void initializeButtons(QHBoxLayout *buttonLayout) {
        // addButton(buttonLayout, "/usr/bin/konsole");
        // addButton(buttonLayout, "/usr/bin/dolphin");
        // addButton(buttonLayout, "/usr/bin/falkon");
        restoreButtons();
        buttonLayout->addStretch(1);
        updateDockGeometry();
    }
    void addButton(QHBoxLayout *layout, const QString &app) {
        auto button = new IconButton(app, app, this);
        QString iconName = findIconForBinary(app);
        QIcon icon;

        QString iconPath = findIconFilePath(app);
        if (!iconName.isEmpty()) {
            if (QFile::exists(iconPath)) {
                qDebug() << "Icon file for" << app.section("/", - 1) << "exist, adding to button. iconPath:" << iconPath;
                icon.addFile(iconPath);
                button->setIcon(makePngScaleble(iconPath));
            } else {
                qDebug() << "Icon file for '" + app.section("/", - 1) + "' does not exist, using theme icon:" << iconName;
                icon = QIcon::fromTheme(iconName);
                button->setIcon(icon);
            }
        }

        // Styling the button to look more like an icon
        button->setStyleSheet(
            "QPushButton {"
            "border: none;" // removes the border
            "background-color: transparent;" // makes the background transparent
            "}"
            "QPushButton:hover {"
            "background-color: rgba(255, 255, 255, 0);" // slightly white background on hover
            "}"
            "QPushButton:pressed {"
            "background-color: rgba(255, 255, 255, 30);" // more white background when pressed
            "}"
        );
        connect(button, &IconButton::clicked, [app]() {
            QProcess::startDetached(app, QStringList());
        });
        buttons.append(button);
        layout->addWidget(button);
        updateDockGeometry();
        saveButtons();
    }
    void removeButton(IconButton *button) {
        if (buttons.removeOne(button)) { // Use removeOne, which returns true if an item was removed
            buttonLayout->removeWidget(button); // Make sure you remove it from the correct layout
            button->deleteLater(); // Schedule the button for deletion
        }
        updateDockGeometry();
        saveButtons();
    }
    void AddButtonRequested() {
        bool ok;
        // Prompt user for the application's path
        QString app = QInputDialog::getText(this, tr("Add New Button"),
                                            tr("Application Path:"), QLineEdit::Normal,
                                            "app", &ok);

        app = "/usr/bin/" + app;
        if (ok && !app.isEmpty()) {
            addButton(buttonLayout, app);
            setButtonSize(current_button_size);
        }
    }
    void setButtonSize(int edgeLength) {
        if (edgeLength <= 0) {
            qWarning() << "Invalid edge length for button size:" << edgeLength;
            return;
        }
        current_button_size = edgeLength;
        int maxButtonHeight = 0;                            // Find the maximum button height for setting the dock height
        for (auto &button : buttons) {
            button->setFixedSize(edgeLength, edgeLength);   // Consistent size
            button->updateSize(edgeLength);
            button->setIconSize(QSize(edgeLength  - 12, edgeLength - 12));
            maxButtonHeight = edgeLength;
        }
        int verticalMargin = edgeLength / 10;               // Adjust the margin as necessary.
        setFixedHeight(maxButtonHeight + verticalMargin);
        moveToEdge(current_placement);
        updateDockGeometry();
    }
    void moveToEdge(const QString &edge) {
        QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
        int x, y;
        if (edge == "Top") {
            current_placement = "Top";
            x = (screenGeometry.width() - width()) / 2;
            y = 0;
        } else if (edge == "Bottom") {
            current_placement = "Buttom";
            x = (screenGeometry.width() - width()) / 2;
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
        move(x, y);
    }
    QString findIconForBinary(const QString& binaryPath) {
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
    QString findIconFilePath(const QString& iconName) {
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
    void openAppAddDialog() {
        AppAddDialog *dialog = new AppAddDialog(this);

        // Handle the accepted signal to add the app
        connect(dialog, &QDialog::accepted, [this, dialog]() {
            // Assuming you have a method to handle adding the app
            QString selectedAppPath = dialog->appPath; // You'll need to implement getSelectedAppPath() to retrieve the selected app path
            addButton(buttonLayout, selectedAppPath);
        });

        dialog->exec();
    }
    QPixmap makePngScaleble(const QString& path) {
        QPixmap originalPixmap(path);
        QPixmap scaledPixmap = originalPixmap.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        return scaledPixmap;
    }
    void saveButtons() {                                    // New function to save buttons configuration
        QStringList appList;
        for (IconButton *button : buttons) {
            appList << button->appPath(); // Assume IconButton has a method to get the app path
            qDebug() << "saveButtons:" << button << button->appPath();
        }
        settings.setValue("buttons", appList);
    }
    void restoreButtons() {                                 // New function to restore buttons on launch
        QStringList appList = settings.value("buttons").toStringList();
        for (const QString &app : appList) {
            addButton(buttonLayout, app);
            qDebug() << "restoreButtons: Loaded in" << app;
        }
    }
    enum class MoveDirection {
        Left,
        Right
    };
    void moveButton(IconButton* button, MoveDirection direction) {
        int index = buttonLayout->indexOf(button);
        int newIndex = (direction == MoveDirection::Left) ? index - 1 : index + 1;

        // Check if new index is within the valid range
        if (newIndex >= 0 && newIndex < buttonLayout->count()) {
            buttonLayout->removeWidget(button);
            buttonLayout->insertWidget(newIndex, button);

        }
        saveButtons();
        updateDockGeometry();
    }


    // Prevent copy and move semantics
    SimpleDock(const SimpleDock &) = delete;
    SimpleDock(SimpleDock &&) = delete;
    SimpleDock &operator=(const SimpleDock &) = delete;
    SimpleDock &operator=(SimpleDock &&) = delete;
protected:
    void contextMenuEvent(QContextMenuEvent *event) override {
        QMenu contextMenu(this);                            // Main ContextMenu

        QWidget *clickedWidget = childAt(event->pos());     // Only add "Remove Button" to contextMenu if right clicked on button
        IconButton *clickedButton = dynamic_cast<IconButton *>(clickedWidget);
        if (clickedButton) {

            QString buttonName = clickedButton->getName(); // Retrieve the name of the button
            // Construct the action text
            QString actionText = QString("Remove %1").arg(buttonName.section("/", -1));
            QAction *removeAction = contextMenu.addAction(actionText);
            connect(removeAction, &QAction::triggered, this, [this, clickedButton]() {
                removeButton(clickedButton);
            });

            QAction* moveLeftAction = contextMenu.addAction("Move Left");
            QAction* moveRightAction = contextMenu.addAction("Move Right");

            connect(moveLeftAction, &QAction::triggered, this, [this, clickedButton]() {
                moveButton(clickedButton, MoveDirection::Left);
            });

            connect(moveRightAction, &QAction::triggered, this, [this, clickedButton]() {
                moveButton(clickedButton, MoveDirection::Right);
            });
        }

        QAction actionAddApp("Add App", this);
        connect(&actionAddApp, &QAction::triggered, this, &::SimpleDock::openAppAddDialog);

        contextMenu.addAction(&actionAddApp);

        QMenu *prefrenceMenu = contextMenu.addMenu("Dock Prefrences");

        // Submenu for Placement
        QMenu *placementMenu = prefrenceMenu->addMenu("Placement");
        placementMenu->addAction("Top", this, [this]() { moveToEdge("Top"); });
        placementMenu->addAction("Bottom", this, [this]() { moveToEdge("Bottom"); });
        placementMenu->addAction("Left", this, [this]() { moveToEdge("Left"); });
        placementMenu->addAction("Right", this, [this]() { moveToEdge("Right"); });

        // Submenu for Button Size
        QMenu *sizeMenu = prefrenceMenu->addMenu("Button Size");
        for (int size = 40; size <= 80; size += 2) {
            QString sizeStr = QString::number(size);
            sizeMenu->addAction(sizeStr, this, [this, size]() { setButtonSize(size + 12); });
        }

        // Submenu for ZoomScale
        QMenu *animDurMenu = prefrenceMenu->addMenu("Animation Duration");
        for (int size = 40; size <= 200; size += 10) {
            QAction *action = animDurMenu->addAction(QString::number(size));
            connect(action, &QAction::triggered, this, [this, size]() {// Connect the action to a lambda function
                for (IconButton *button : buttons) { // Iterate over all IconButton instances and update their scale factor
                    button->updateAnimDuration(size);
                }
            });
        }

        // Submenu for ZoomScale
        QMenu *ZoomMenu = prefrenceMenu->addMenu("Zoom Scale");
        for (int size = 100; size <= 160; size += 5) {
            // Calculate the scale factor as a float
            float scale = size / 100.0f;
            // Convert the integer to a string and create the menu action
            QAction *action = ZoomMenu->addAction(QString::number(scale));
            // Connect the action to a lambda function
            connect(action, &QAction::triggered, this, [this, scale]() {
                // Iterate over all IconButton instances and update their scale factor
                for (IconButton *button : buttons) {
                    button->updateScaleFactor(scale);
                }
            });
        }

        contextMenu.exec(event->globalPos());
    }
    // void mousePressEvent(QMouseEvent *event) override {
    //     if (event->button() == Qt::LeftButton) {
    //         dragPosition = event->globalPos() - frameGeometry().topLeft();
    //     }
    // }
    // void mouseMoveEvent(QMouseEvent *event) override {
    //     if (event->buttons() & Qt::LeftButton) {
    //         QPoint newPosition = event->globalPos() - dragPosition;
    //         // Lock the Y-coordinate to the current window Y-coordinate
    //         move(newPosition.x(), this->y());
    //     }
    // }
};


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    SimpleDock dock;
    dock.show();
    dock.moveToEdge("Bottom");
    return app.exec();
}
