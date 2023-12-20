#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include "dockConfig.h"
#include "qobject.h"
#include "highResAnimation.h"
#include "qpoint.h"
#include "qpropertyanimation.h"
#include <QPushButton>
#include <QPropertyAnimation>
#include <QApplication>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QEvent>

class IconButton : public QPushButton {
    Q_OBJECT
    std::function<QWidget*()> windowFactory;
    QWidget *windowToLaunch = nullptr;
    QString name_; // This should already be a part of your IconButton class
    QString appPath_; // Private member to store the application path
public:
    explicit IconButton(const QString& name, const QString& appPath , QWidget *parent = nullptr);
    ~IconButton();
    QString appPath() const;
    QString getName() const;
    void bounceIcon();
    QVariantAnimation *ZoomAnimation = new QVariantAnimation(this);
    QPropertyAnimation *maxSize = new QPropertyAnimation(this, "maximumSize");
    QPropertyAnimation *animation = new QPropertyAnimation(this, "iconSize");
    QPropertyAnimation *bounceAnimation = new QPropertyAnimation(this, "iconSize");
    QPropertyAnimation *posAnimation = new QPropertyAnimation(this, "pos");
    QWidget *currentButton;
    void animateSize(const QSize &newSize);
    void animateButtonSize(const int &endValue);
    void animatePos();

// public slots:
//     void sendChange(const int&num);
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    QSize iconSize() const;
    QSize buttonSize() const;
    QPoint currentPos() const;
    int curSize() const;
private:
    int current_size = 0;
    QPoint ogPos = QPoint();
signals:
    void hoverEntered();
    void hoverLeft();
    double sizeChanged(const QVariant &num);
};

#endif // ICONBUTTON_H
