#ifndef HEALTHBARWIDGET_H
#define HEALTHBARWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>

class HealthBarWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int currentHealth READ currentHealth WRITE setCurrentHealth)

public:
    explicit HealthBarWidget(QWidget* parent = nullptr);
    
    void setHealth(int health, int maxHealth);
    void setMaxHealth(int maxHealth);
    
    int currentHealth() const { return m_currentHealth; }
    void setCurrentHealth(int health);

protected:
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    int m_currentHealth;
    int m_maxHealth;
    int m_displayHealth; // For animation
    QPropertyAnimation* m_animation;
    
    QColor getHealthColor() const;
};

#endif // HEALTHBARWIDGET_H
