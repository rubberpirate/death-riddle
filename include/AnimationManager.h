#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <QObject>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>

class AnimationManager : public QObject {
    Q_OBJECT

public:
    explicit AnimationManager(QObject* parent = nullptr);
    virtual ~AnimationManager() = default;

    // Utility animation creation
    static QPropertyAnimation* createFadeAnimation(QWidget* widget, int duration, qreal startOpacity, qreal endOpacity);
    static QPropertyAnimation* createSlideAnimation(QWidget* widget, int duration, QPoint startPos, QPoint endPos);
    static QPropertyAnimation* createScaleAnimation(QWidget* widget, int duration, qreal startScale, qreal endScale);
    
    // Effect animations
    static void pulseEffect(QWidget* widget, int duration = 500);
    static void shakeEffect(QWidget* widget, int intensity = 10, int duration = 300);
    static void fadeInWidget(QWidget* widget, int duration = 300);
    static void fadeOutWidget(QWidget* widget, int duration = 300);
};

#endif // ANIMATIONMANAGER_H
