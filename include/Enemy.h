#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QMap>
#include "AnimatedSprite.h"

class Enemy : public QObject {
    Q_OBJECT

public:
    enum class Type {
        PINK_MONSTER,
        OWLET_MONSTER
    };
    
    enum class State {
        IDLE,
        WALKING,
        HURT,
        DEAD
    };

    explicit Enemy(Type type, const QPointF& position, int riddleId = -1, QObject* parent = nullptr);
    ~Enemy();
    
    // Position and movement
    QPointF position() const { return m_position; }
    void setPosition(const QPointF& pos);
    QRectF boundingBox() const;
    
    // State
    State state() const { return m_state; }
    void setState(State state);
    bool isDead() const { return m_state == State::DEAD; }
    bool isDeathAnimationFinished() const;
    
    // Update
    void update(float deltaTime);
    
    // Get current animation sprite
    AnimatedSprite* getCurrentSprite() { return m_animations[m_state]; }
    bool isFacingRight() const { return m_facingRight; }
    
    // Riddle association
    int riddleId() const { return m_riddleId; }
    void setRiddleId(int id) { m_riddleId = id; }
    bool isRiddleTriggered() const { return m_riddleTriggered; }
    void setRiddleTriggered(bool triggered) { m_riddleTriggered = triggered; }
    
    // Actions
    void takeDamage();
    void die();
    
    // Size constants
    static constexpr float WIDTH = 32.0f;
    static constexpr float HEIGHT = 32.0f;

signals:
    void died();
    void deathAnimationComplete();

private:
    void loadAnimations();
    void updateAnimation(float deltaTime);
    void patrol();

private:
    Type m_type;
    QPointF m_position;
    State m_state;
    bool m_facingRight;
    int m_riddleId;
    bool m_riddleTriggered;  // Prevents re-triggering riddle
    
    // Patrol behavior
    float m_patrolDistance;
    QPointF m_startPosition;
    float m_moveSpeed;
    
    // Animations
    QMap<State, AnimatedSprite*> m_animations;
};

#endif // ENEMY_H
