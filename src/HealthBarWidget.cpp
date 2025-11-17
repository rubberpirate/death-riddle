#include "HealthBarWidget.h"
#include <QPainter>
#include <QPaintEvent>

HealthBarWidget::HealthBarWidget(QWidget* parent)
    : QWidget(parent)
    , m_currentHealth(100)
    , m_maxHealth(100)
    , m_displayHealth(100)
{
    m_animation = new QPropertyAnimation(this, "currentHealth");
    m_animation->setDuration(500);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
    
    setMinimumHeight(30);
}

void HealthBarWidget::setHealth(int health, int maxHealth) {
    m_maxHealth = maxHealth;
    
    if (health != m_displayHealth) {
        m_animation->stop();
        m_animation->setStartValue(m_displayHealth);
        m_animation->setEndValue(health);
        m_animation->start();
    }
}

void HealthBarWidget::setMaxHealth(int maxHealth) {
    m_maxHealth = maxHealth;
    update();
}

void HealthBarWidget::setCurrentHealth(int health) {
    m_displayHealth = health;
    update();
}

void HealthBarWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int width = this->width();
    int height = this->height();

    // Background
    painter.fillRect(rect(), QColor(30, 30, 30));

    // Border
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawRect(1, 1, width - 2, height - 2);

    // Health bar
    if (m_maxHealth > 0) {
        float percentage = static_cast<float>(m_displayHealth) / m_maxHealth;
        int healthWidth = static_cast<int>((width - 4) * percentage);

        QColor healthColor = getHealthColor();
        painter.fillRect(2, 2, healthWidth, height - 4, healthColor);

        // Shine effect
        QLinearGradient gradient(0, 0, 0, height);
        gradient.setColorAt(0, QColor(255, 255, 255, 80));
        gradient.setColorAt(0.5, QColor(255, 255, 255, 0));
        gradient.setColorAt(1, QColor(0, 0, 0, 50));
        painter.fillRect(2, 2, healthWidth, height - 4, gradient);
    }

    // Text
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignCenter, 
                     QString("HP: %1 / %2").arg(m_displayHealth).arg(m_maxHealth));
}

QColor HealthBarWidget::getHealthColor() const {
    if (m_maxHealth == 0) return Qt::gray;
    
    float percentage = static_cast<float>(m_displayHealth) / m_maxHealth;
    
    if (percentage > 0.6f) {
        return QColor(50, 200, 50); // Green
    } else if (percentage > 0.3f) {
        return QColor(255, 200, 0); // Orange
    } else {
        return QColor(220, 50, 50); // Red
    }
}

QSize HealthBarWidget::sizeHint() const {
    return QSize(300, 30);
}

QSize HealthBarWidget::minimumSizeHint() const {
    return QSize(150, 25);
}
