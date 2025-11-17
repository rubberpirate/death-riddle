#include "AnimationManager.h"
#include <QWidget>
#include <QGraphicsOpacityEffect>

AnimationManager::AnimationManager(QObject* parent)
    : QObject(parent)
{
}

QPropertyAnimation* AnimationManager::createFadeAnimation(QWidget* widget, int duration, qreal startOpacity, qreal endOpacity) {
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);
    
    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(duration);
    animation->setStartValue(startOpacity);
    animation->setEndValue(endOpacity);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    
    return animation;
}

QPropertyAnimation* AnimationManager::createSlideAnimation(QWidget* widget, int duration, QPoint startPos, QPoint endPos) {
    QPropertyAnimation* animation = new QPropertyAnimation(widget, "pos");
    animation->setDuration(duration);
    animation->setStartValue(startPos);
    animation->setEndValue(endPos);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    
    return animation;
}

QPropertyAnimation* AnimationManager::createScaleAnimation(QWidget* widget, int duration, qreal startScale, qreal endScale) {
    QPropertyAnimation* animation = new QPropertyAnimation(widget, "geometry");
    animation->setDuration(duration);
    animation->setEasingCurve(QEasingCurve::OutElastic);
    
    return animation;
}

void AnimationManager::pulseEffect(QWidget* widget, int duration) {
    QGraphicsOpacityEffect* effect = qobject_cast<QGraphicsOpacityEffect*>(widget->graphicsEffect());
    if (!effect) {
        effect = new QGraphicsOpacityEffect(widget);
        widget->setGraphicsEffect(effect);
    }
    
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup(widget);
    
    QPropertyAnimation* fadeOut = new QPropertyAnimation(effect, "opacity");
    fadeOut->setDuration(duration / 2);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.5);
    
    QPropertyAnimation* fadeIn = new QPropertyAnimation(effect, "opacity");
    fadeIn->setDuration(duration / 2);
    fadeIn->setStartValue(0.5);
    fadeIn->setEndValue(1.0);
    
    group->addAnimation(fadeOut);
    group->addAnimation(fadeIn);
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void AnimationManager::shakeEffect(QWidget* widget, int intensity, int duration) {
    QPoint originalPos = widget->pos();
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup(widget);
    
    int steps = 6;
    int stepDuration = duration / steps;
    
    for (int i = 0; i < steps; ++i) {
        QPropertyAnimation* shake = new QPropertyAnimation(widget, "pos");
        shake->setDuration(stepDuration);
        
        int offsetX = (i % 2 == 0 ? intensity : -intensity);
        shake->setEndValue(originalPos + QPoint(offsetX, 0));
        
        group->addAnimation(shake);
    }
    
    QPropertyAnimation* restore = new QPropertyAnimation(widget, "pos");
    restore->setDuration(stepDuration);
    restore->setEndValue(originalPos);
    group->addAnimation(restore);
    
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void AnimationManager::fadeInWidget(QWidget* widget, int duration) {
    QGraphicsOpacityEffect* effect = qobject_cast<QGraphicsOpacityEffect*>(widget->graphicsEffect());
    if (!effect) {
        effect = new QGraphicsOpacityEffect(widget);
        widget->setGraphicsEffect(effect);
    }
    
    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(duration);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    
    widget->show();
}

void AnimationManager::fadeOutWidget(QWidget* widget, int duration) {
    QGraphicsOpacityEffect* effect = qobject_cast<QGraphicsOpacityEffect*>(widget->graphicsEffect());
    if (!effect) {
        effect = new QGraphicsOpacityEffect(widget);
        widget->setGraphicsEffect(effect);
    }
    
    QPropertyAnimation* animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(duration);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    
    QObject::connect(animation, &QPropertyAnimation::finished, widget, &QWidget::hide);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
