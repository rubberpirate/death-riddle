#ifndef PLAYER2D_H
#define PLAYER2D_H

#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QTimer>
#include <QMap>
#include "AnimatedSprite.h"

class Player2D : public QObject {
    Q_OBJECT

public:
    enum class State {
        IDLE,
        RUNNING_LEFT,
        RUNNING_RIGHT,
        JUMPING,
        FALLING,
        HURT,
        DEAD
    };

    explicit Player2D(QObject* parent = nullptr);
    
    // Position and movement
    QPointF position() const { return m_position; }
    void setPosition(const QPointF& pos);
    QRectF boundingBox() const;
    
    // Physics
    QPointF velocity() const { return m_velocity; }
    void setVelocity(const QPointF& vel) { m_velocity = vel; }
    
    // Movement controls
    void moveLeft();
    void moveRight();
    void jump();
    void stopHorizontalMovement();
    
    // State
    State state() const { return m_state; }
    void setState(State state);
    
    // Physics update
    void update(float deltaTime);
    void applyGravity(float deltaTime);
    void setOnGround(bool onGround);
    bool isOnGround() const { return m_onGround; }
    
    // Health and lives
    int health() const { return m_health; }
    int lives() const { return m_lives; }
    void takeDamage(int amount);
    void heal(int amount);
    void die();
    void respawn(const QPointF& spawnPoint);
    
    // Score and collectibles
    int score() const { return m_score; }
    void addScore(int points);
    int coinsCollected() const { return m_coins; }
    void collectCoin();
    
    // Size constants
    static constexpr float WIDTH = 32.0f;
    static constexpr float HEIGHT = 32.0f;

signals:
    void positionChanged(QPointF position);
    void stateChanged(State state);
    void healthChanged(int health);
    void livesChanged(int lives);
    void scoreChanged(int score);
    void coinsChanged(int coins);
    void died();
    void respawned();

public:
    // Get current animation sprite
    AnimatedSprite* getCurrentSprite() { return m_animations[m_state]; }
    bool isFacingRight() const { return m_facingRight; }

private:
    void loadAnimations();
    void updateAnimation(float deltaTime);

private:
    QPointF m_position;
    QPointF m_velocity;
    QPointF m_spawnPoint;
    State m_state;
    
    bool m_onGround;
    bool m_canJump;
    bool m_facingRight;
    
    // Double jump mechanics
    bool m_hasDoubleJump;
    float m_doubleJumpCooldown;
    static constexpr float DOUBLE_JUMP_COOLDOWN_TIME = 3.0f;  // 3 second cooldown
    
    int m_health;
    int m_lives;
    int m_score;
    int m_coins;
    
    // Animations
    QMap<State, AnimatedSprite*> m_animations;
    
    // Physics constants (properly scaled for smooth gameplay at 60fps)
    static constexpr float GRAVITY = 800.0f;  // Gravity acceleration (pixels/s²)
    static constexpr float FRICTION = 0.85f;  // Friction multiplier for deceleration
    static constexpr float ACCELERATION_X = 600.0f;  // Horizontal acceleration (pixels/s²)
    static constexpr float JUMP_VELOCITY = -350.0f;  // Initial jump velocity (negative = up)
    static constexpr float MAX_SPEED_X = 200.0f;  // Max horizontal speed (pixels/s)
    static constexpr float MAX_SPEED_Y = 500.0f;  // Max fall speed (pixels/s)
};

#endif // PLAYER2D_H
