#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include "Player.h"
#include <QWidget>
#include <QPropertyAnimation>

class PlayerWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(qreal wobbleAngle READ wobbleAngle WRITE setWobbleAngle)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)

public:
    explicit PlayerWidget(QWidget* parent = nullptr);
    
    void setPlayerState(Player::State state);
    
    qreal wobbleAngle() const { return m_wobbleAngle; }
    void setWobbleAngle(qreal angle);
    
    qreal scale() const { return m_scale; }
    void setScale(qreal scale);

protected:
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;

private:
    void playIdleAnimation();
    void playDamagedAnimation();
    void playCelebratingAnimation();
    void playDefeatAnimation();

    Player::State m_state;
    qreal m_wobbleAngle;
    qreal m_scale;
    QPropertyAnimation* m_wobbleAnimation;
    QPropertyAnimation* m_scaleAnimation;
};

#endif // PLAYERWIDGET_H
