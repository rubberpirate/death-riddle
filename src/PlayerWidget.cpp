#include "PlayerWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QSequentialAnimationGroup>
#include <cmath>

PlayerWidget::PlayerWidget(QWidget* parent)
    : QWidget(parent)
    , m_state(Player::State::IDLE)
    , m_wobbleAngle(0)
    , m_scale(1.0)
{
    m_wobbleAnimation = new QPropertyAnimation(this, "wobbleAngle");
    m_scaleAnimation = new QPropertyAnimation(this, "scale");
    
    setMinimumSize(100, 150);
    playIdleAnimation();
}

void PlayerWidget::setPlayerState(Player::State state) {
    if (m_state == state) return;
    
    m_state = state;
    
    m_wobbleAnimation->stop();
    m_scaleAnimation->stop();
    
    switch (state) {
        case Player::State::IDLE:
        case Player::State::THINKING:
            playIdleAnimation();
            break;
        case Player::State::DAMAGED:
            playDamagedAnimation();
            break;
        case Player::State::CELEBRATING:
            playCelebratingAnimation();
            break;
        case Player::State::DEFEATED:
            playDefeatAnimation();
            break;
    }
}

void PlayerWidget::setWobbleAngle(qreal angle) {
    m_wobbleAngle = angle;
    update();
}

void PlayerWidget::setScale(qreal scale) {
    m_scale = scale;
    update();
}

void PlayerWidget::playIdleAnimation() {
    m_wobbleAnimation->setDuration(2000);
    m_wobbleAnimation->setStartValue(-5.0);
    m_wobbleAnimation->setEndValue(5.0);
    m_wobbleAnimation->setEasingCurve(QEasingCurve::InOutSine);
    m_wobbleAnimation->setLoopCount(-1);
    m_wobbleAnimation->start();
    
    m_scale = 1.0;
}

void PlayerWidget::playDamagedAnimation() {
    // Quick shake animation
    QSequentialAnimationGroup* shakeGroup = new QSequentialAnimationGroup(this);
    
    for (int i = 0; i < 3; ++i) {
        QPropertyAnimation* shake1 = new QPropertyAnimation(this, "wobbleAngle");
        shake1->setDuration(50);
        shake1->setStartValue(-15.0);
        shake1->setEndValue(15.0);
        shakeGroup->addAnimation(shake1);
        
        QPropertyAnimation* shake2 = new QPropertyAnimation(this, "wobbleAngle");
        shake2->setDuration(50);
        shake2->setStartValue(15.0);
        shake2->setEndValue(-15.0);
        shakeGroup->addAnimation(shake2);
    }
    
    connect(shakeGroup, &QSequentialAnimationGroup::finished, this, &PlayerWidget::playIdleAnimation);
    shakeGroup->start(QAbstractAnimation::DeleteWhenStopped);
    
    // Flash scale
    m_scaleAnimation->setDuration(300);
    m_scaleAnimation->setStartValue(1.0);
    m_scaleAnimation->setKeyValueAt(0.5, 0.8);
    m_scaleAnimation->setEndValue(1.0);
    m_scaleAnimation->start();
}

void PlayerWidget::playCelebratingAnimation() {
    // Jump animation
    m_scaleAnimation->setDuration(1000);
    m_scaleAnimation->setStartValue(1.0);
    m_scaleAnimation->setKeyValueAt(0.3, 1.2);
    m_scaleAnimation->setKeyValueAt(0.6, 1.0);
    m_scaleAnimation->setKeyValueAt(0.8, 1.1);
    m_scaleAnimation->setEndValue(1.0);
    m_scaleAnimation->setLoopCount(2);
    m_scaleAnimation->start();
    
    connect(m_scaleAnimation, &QPropertyAnimation::finished, this, &PlayerWidget::playIdleAnimation);
}

void PlayerWidget::playDefeatAnimation() {
    // Fall animation
    m_scaleAnimation->setDuration(1500);
    m_scaleAnimation->setStartValue(1.0);
    m_scaleAnimation->setEndValue(0.3);
    m_scaleAnimation->setEasingCurve(QEasingCurve::InCubic);
    m_scaleAnimation->start();
    
    m_wobbleAnimation->setDuration(1500);
    m_wobbleAnimation->setStartValue(0.0);
    m_wobbleAnimation->setEndValue(90.0);
    m_wobbleAnimation->setEasingCurve(QEasingCurve::InCubic);
    m_wobbleAnimation->start();
}

void PlayerWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int centerX = width() / 2;
    int centerY = height() / 2;
    
    // Apply transformations
    painter.translate(centerX, centerY);
    painter.scale(m_scale, m_scale);
    painter.rotate(m_wobbleAngle);
    painter.translate(-centerX, -centerY);
    
    // Draw player character (simple humanoid figure)
    int baseY = centerY + 30;
    int headRadius = 20;
    int bodyHeight = 40;
    int bodyWidth = 30;
    
    // Shadow
    painter.setBrush(QColor(0, 0, 0, 50));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(centerX - 25, baseY + 40, 50, 15);
    
    // Head
    painter.setBrush(QColor(255, 220, 177));
    painter.setPen(QPen(QColor(200, 150, 100), 2));
    painter.drawEllipse(centerX - headRadius, centerY - 40, headRadius * 2, headRadius * 2);
    
    // Eyes
    painter.setBrush(Qt::white);
    painter.drawEllipse(centerX - 12, centerY - 35, 8, 8);
    painter.drawEllipse(centerX + 4, centerY - 35, 8, 8);
    
    // Pupils
    painter.setBrush(Qt::black);
    painter.drawEllipse(centerX - 9, centerY - 33, 4, 4);
    painter.drawEllipse(centerX + 7, centerY - 33, 4, 4);
    
    // Mouth (changes based on state)
    painter.setPen(QPen(Qt::black, 2));
    if (m_state == Player::State::CELEBRATING) {
        // Smile
        painter.drawArc(centerX - 10, centerY - 25, 20, 15, 0, -180 * 16);
    } else if (m_state == Player::State::DAMAGED || m_state == Player::State::DEFEATED) {
        // Frown
        painter.drawArc(centerX - 10, centerY - 18, 20, 10, 0, 180 * 16);
    } else {
        // Neutral
        painter.drawLine(centerX - 8, centerY - 22, centerX + 8, centerY - 22);
    }
    
    // Body
    painter.setBrush(QColor(70, 130, 180));
    painter.setPen(QPen(QColor(50, 100, 150), 2));
    painter.drawRoundedRect(centerX - bodyWidth/2, centerY - 15, bodyWidth, bodyHeight, 5, 5);
    
    // Arms
    painter.setPen(QPen(QColor(70, 130, 180), 6, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(centerX - bodyWidth/2, centerY - 10, centerX - bodyWidth/2 - 15, centerY + 10);
    painter.drawLine(centerX + bodyWidth/2, centerY - 10, centerX + bodyWidth/2 + 15, centerY + 10);
    
    // Legs
    painter.drawLine(centerX - 8, centerY + bodyHeight - 15, centerX - 12, baseY + 30);
    painter.drawLine(centerX + 8, centerY + bodyHeight - 15, centerX + 12, baseY + 30);
}

QSize PlayerWidget::sizeHint() const {
    return QSize(100, 150);
}
