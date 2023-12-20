#include "IconButton.h"
#include "SimpleDock.h"
#include "dockConfig.h"
#include "debug.h"
#include "highResAnimation.h"
#include "highResAnimation.h"
#include "qdebug.h"
#include "qlayout.h"
#include "qlogging.h"
#include "qnamespace.h"
#include "qobject.h"
#include "qpoint.h"
#include "qpropertyanimation.h"
#include "qpushbutton.h"
#include "qsize.h"
#include "qsizepolicy.h"
#include "qvariant.h"
#include "qvariantanimation.h"
#include "qwidget.h"
#include <QSequentialAnimationGroup>


std::function<QWidget*()> windowFactory;
QWidget *windowToLaunch = nullptr;

IconButton::IconButton(const QString& name, const QString& appPath , QWidget *parent)
    : QPushButton(parent), name_(name), appPath_(appPath) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QObject::connect(bounceAnimation, &QPropertyAnimation::valueChanged, [=]() { setFixedSize(iconSize()); });
        QObject::connect(animation, &QVariantAnimation::valueChanged, [=]() { setFixedSize(iconSize()); });
    }
IconButton::~IconButton() {
    delete animation;
}
QString IconButton::appPath() const {                               // Implement the appPath() accessor method
    return appPath_;
}
QString IconButton::getName() const {
    return name_;
}

void IconButton::enterEvent(QEnterEvent *event) {
    animateSize(QSize(num(BUTTON_SIZE) * doubleNum(FLOAT_SCALE_FACTOR), num(BUTTON_SIZE) * doubleNum(FLOAT_SCALE_FACTOR)));
    QPushButton::enterEvent(event);
}
void IconButton::leaveEvent(QEvent *event) {
    animateSize(Qnum(BUTTON_SIZE));
    QPushButton::leaveEvent(event);
}
QSize IconButton::iconSize() const {
    return QPushButton::iconSize();
}
QSize IconButton::buttonSize() const {
    return this->size();
}
int IconButton::curSize() const {
    return QPushButton::size().height();
}
QPoint IconButton::currentPos() const {
    return this->pos();
}

void IconButton::bounceIcon() {
    // Setting up the keyframes for the bounce effect
    bounceAnimation->setKeyValues({{0, Qnum(ICON_SIZE) * doubleNum(FLOAT_SCALE_FACTOR)}, {0.5, (Qnum(ICON_SIZE) * doubleNum(FLOAT_SCALE_FACTOR)) + Qnum(BOUNCE_ICON_SIZE)}, {1, Qnum(ICON_SIZE) * doubleNum(FLOAT_SCALE_FACTOR)}});
    bounceAnimation->setDuration(num(BOUNCE_ANIM_DUR));
    bounceAnimation->start();
}
void IconButton::animateSize(const QSize &newSize) {
    if(animation->state() == QAbstractAnimation::Running) {
        animation->stop();
    }

    animation->setDuration(num(ZOOM_ANIM_DUR));
    animation->setStartValue(iconSize());
    animation->setEndValue(newSize);
    animation->setEasingCurve(QEasingCurve::InOutCubic);
    animation->start();

}

void IconButton::animateButtonSize(const int &endValue) {
    if(animation->state() == QAbstractAnimation::Running) {
        animation->stop();
    }
    animation->setDuration(num(ZOOM_ANIM_DUR));
    animation->setStartValue(iconSize().width());
    animation->setEndValue(endValue);
    animation->start();
}
void IconButton::animatePos() {
    if(posAnimation->state() == QAbstractAnimation::Running) {
        posAnimation->stop();
    }

    posAnimation->setKeyValues({{0, QPoint(pos().x(), pos().y())},{ 0.5, QPoint(pos().x(), pos().y() + 10)},{ 1, QPoint(pos().x(), pos().y())}});
    posAnimation->setDuration(num(ZOOM_ANIM_DUR));
    // debug(this->name_, currentPos());
    // QPoint endPos = QPoint(currentPos().x(), newSize.y());
    // posAnimation->setStartValue(QPoint(currentPos()));
    // posAnimation->setEndValue(QPoint(endPos));

    // posAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    posAnimation->start();
}
// void IconButton::sendChange(const int&num){
//     emit sizeChanged(num);
// }

// void IconButton::mousePressEvent(QMouseEvent *event) {
//     QPushButton::mousePressEvent(event); // Call base class handler
//     bounceIcon(); // Start the bounce animation
//     // You can launch the application here or after the animation finishes
// }
