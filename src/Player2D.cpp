#include "Player2D.h"
#include <algorithm>
#include <QDebug>

Player2D::Player2D(QObject* parent)
    : QObject(parent)
    , m_position(100, 100)
    , m_velocity(0, 0)
    , m_spawnPoint(100, 100)
    , m_state(State::IDLE)
    , m_onGround(false)
    , m_canJump(true)
    , m_facingRight(true)
    , m_hasDoubleJump(true)
    , m_doubleJumpCooldown(0.0f)
    , m_health(100)
    , m_lives(3)
    , m_score(0)
    , m_coins(0)
{
    loadAnimations();
}

void Player2D::loadAnimations() {
    // Load sprite sheets for different animations
    QString basePath = "assets/sprites/";
    
    // Idle animation (4 frames)
    AnimatedSprite* idleSprite = new AnimatedSprite();
    if (idleSprite->loadSpriteSheet(basePath + "Dude_Monster_Idle_4.png", 32, 32, 4)) {
        idleSprite->setFrameRate(10.0f);  // Increased from 8 to 10 FPS
        idleSprite->setLoop(true);
        idleSprite->play();
        m_animations[State::IDLE] = idleSprite;
    } else {
        qDebug() << "Failed to load idle animation";
        delete idleSprite;
    }
    
    // Run animation (6 frames) - used for both left and right
    AnimatedSprite* runSprite = new AnimatedSprite();
    if (runSprite->loadSpriteSheet(basePath + "Dude_Monster_Run_6.png", 32, 32, 6)) {
        runSprite->setFrameRate(18.0f);  // Increased from 12 to 18 FPS
        runSprite->setLoop(true);
        runSprite->play();
        m_animations[State::RUNNING_LEFT] = runSprite;
        m_animations[State::RUNNING_RIGHT] = runSprite;
    } else {
        qDebug() << "Failed to load run animation";
        delete runSprite;
    }
    
    // Jump animation (8 frames)
    AnimatedSprite* jumpSprite = new AnimatedSprite();
    if (jumpSprite->loadSpriteSheet(basePath + "Dude_Monster_Jump_8.png", 32, 32, 8)) {
        jumpSprite->setFrameRate(20.0f);  // Increased from 15 to 20 FPS
        jumpSprite->setLoop(false);
        jumpSprite->play();
        m_animations[State::JUMPING] = jumpSprite;
        m_animations[State::FALLING] = jumpSprite;
    } else {
        qDebug() << "Failed to load jump animation";
        delete jumpSprite;
    }
    
    // Hurt animation (4 frames)
    AnimatedSprite* hurtSprite = new AnimatedSprite();
    if (hurtSprite->loadSpriteSheet(basePath + "Dude_Monster_Hurt_4.png", 32, 32, 4)) {
        hurtSprite->setFrameRate(15.0f);  // Increased from 10 to 15 FPS
        hurtSprite->setLoop(false);
        m_animations[State::HURT] = hurtSprite;
    } else {
        qDebug() << "Failed to load hurt animation";
        delete hurtSprite;
    }
    
    // Death animation (8 frames)
    AnimatedSprite* deathSprite = new AnimatedSprite();
    if (deathSprite->loadSpriteSheet(basePath + "Dude_Monster_Death_8.png", 32, 32, 8)) {
        deathSprite->setFrameRate(12.0f);  // Increased from 10 to 12 FPS
        deathSprite->setLoop(false);
        m_animations[State::DEAD] = deathSprite;
    } else {
        qDebug() << "Failed to load death animation";
        delete deathSprite;
    }
}

void Player2D::updateAnimation(float deltaTime) {
    if (m_animations.contains(m_state)) {
        m_animations[m_state]->update(deltaTime);
    }
}

void Player2D::setPosition(const QPointF& pos) {
    if (m_position != pos) {
        m_position = pos;
        emit positionChanged(m_position);
    }
}

QRectF Player2D::boundingBox() const {
    return QRectF(m_position.x(), m_position.y(), WIDTH, HEIGHT);
}

void Player2D::moveLeft() {
    // Apply acceleration to the left (Mario-like physics)
    m_velocity.setX(m_velocity.x() - ACCELERATION_X);
    if (m_velocity.x() < -MAX_SPEED_X) {
        m_velocity.setX(-MAX_SPEED_X);
    }
    m_facingRight = false;
    if (m_state != State::JUMPING && m_state != State::FALLING && m_state != State::HURT) {
        setState(State::RUNNING_LEFT);
    }
}

void Player2D::moveRight() {
    // Apply acceleration to the right (Mario-like physics)
    m_velocity.setX(m_velocity.x() + ACCELERATION_X);
    if (m_velocity.x() > MAX_SPEED_X) {
        m_velocity.setX(MAX_SPEED_X);
    }
    m_facingRight = true;
    if (m_state != State::JUMPING && m_state != State::FALLING && m_state != State::HURT) {
        setState(State::RUNNING_RIGHT);
    }
}

void Player2D::jump() {
    // Normal jump when on ground
    if (m_onGround && m_canJump && m_state != State::HURT) {
        m_velocity.setY(JUMP_VELOCITY);  // Use JUMP_VELOCITY (negative value for upward)
        m_onGround = false;
        m_canJump = false;  // Prevent infinite jumping
        setState(State::JUMPING);
        if (m_animations.contains(State::JUMPING)) {
            m_animations[State::JUMPING]->reset();
            m_animations[State::JUMPING]->play();
        }
    }
    // Double jump when in air (if available and off cooldown)
    else if (!m_onGround && m_hasDoubleJump && m_doubleJumpCooldown <= 0.0f && m_state != State::HURT) {
        m_velocity.setY(JUMP_VELOCITY);  // Same velocity as normal jump
        m_hasDoubleJump = false;  // Use up the double jump
        m_doubleJumpCooldown = DOUBLE_JUMP_COOLDOWN_TIME;  // Start cooldown
        setState(State::JUMPING);
        if (m_animations.contains(State::JUMPING)) {
            m_animations[State::JUMPING]->reset();
            m_animations[State::JUMPING]->play();
        }
    }
}

void Player2D::stopHorizontalMovement() {
    // Apply friction (Mario-like)
    m_velocity.setX(m_velocity.x() * FRICTION);
    if (std::abs(m_velocity.x()) < 0.1f) {
        m_velocity.setX(0);
    }
    if (m_onGround && m_state != State::DEAD && m_state != State::HURT && std::abs(m_velocity.x()) < 1.0f) {
        setState(State::IDLE);
    }
}

void Player2D::setState(State state) {
    if (m_state != state) {
        m_state = state;
        // Reset animation when state changes
        if (m_animations.contains(state)) {
            m_animations[state]->reset();
            m_animations[state]->play();
        }
        emit stateChanged(state);
    }
}

void Player2D::update(float deltaTime) {
    // Update animation
    updateAnimation(deltaTime);
    
    // Update double jump cooldown
    if (m_doubleJumpCooldown > 0.0f) {
        m_doubleJumpCooldown -= deltaTime;
        if (m_doubleJumpCooldown < 0.0f) {
            m_doubleJumpCooldown = 0.0f;
        }
    }
    
    if (m_state == State::DEAD) {
        return;
    }
    
    // Update position based on velocity
    QPointF newPos = m_position + m_velocity * deltaTime;
    
    // Apply screen boundaries - prevent going off left edge
    if (newPos.x() < 0) {
        newPos.setX(0);
        m_velocity.setX(0);  // Stop horizontal movement at boundary
    }
    
    setPosition(newPos);
    
    // Check if fallen off bottom of screen (death boundary)
    if (newPos.y() > 800) {  // Below screen bottom
        takeDamage(m_health);  // Kill player
        return;
    }
    
    // Cap vertical velocity (Mario-like)
    if (m_velocity.y() > MAX_SPEED_Y) {
        m_velocity.setY(MAX_SPEED_Y);
    } else if (m_velocity.y() < -MAX_SPEED_Y) {
        // Also cap upward velocity to prevent infinite jumping
        m_velocity.setY(-MAX_SPEED_Y);
    }
    
    // Update state based on velocity
    if (m_state != State::HURT) {
        if (!m_onGround) {
            if (m_velocity.y() < 0) {
                if (m_state != State::JUMPING) {
                    setState(State::JUMPING);
                }
            } else if (m_velocity.y() > 0) {
                setState(State::FALLING);
            }
        } else {
            if (std::abs(m_velocity.x()) > 1.0f) {
                if (m_velocity.x() < 0) {
                    setState(State::RUNNING_LEFT);
                } else {
                    setState(State::RUNNING_RIGHT);
                }
            } else {
                setState(State::IDLE);
            }
        }
    }
}

void Player2D::applyGravity(float deltaTime) {
    if (!m_onGround && m_state != State::DEAD) {
        // Apply gravity (Mario-like - constant acceleration)
        m_velocity.setY(m_velocity.y() + GRAVITY * deltaTime);
        // Cap velocity
        if (m_velocity.y() > MAX_SPEED_Y) {
            m_velocity.setY(MAX_SPEED_Y);
        }
    }
}

void Player2D::setOnGround(bool onGround) {
    bool wasOnGround = m_onGround;
    m_onGround = onGround;
    
    if (onGround && !wasOnGround) {
        // Just landed - reset jump abilities
        m_velocity.setY(0);
        m_canJump = true;
        m_hasDoubleJump = true;  // Restore double jump when landing
    }
}

void Player2D::takeDamage(int amount) {
    m_health = std::max(0, m_health - amount);
    emit healthChanged(m_health);
    
    if (m_health <= 0) {
        die();
    }
}

void Player2D::heal(int amount) {
    m_health = std::min(100, m_health + amount);
    emit healthChanged(m_health);
}

void Player2D::die() {
    setState(State::DEAD);
    m_velocity = QPointF(0, 0);
    m_lives--;
    emit livesChanged(m_lives);
    emit died();
}

void Player2D::respawn(const QPointF& spawnPoint) {
    m_spawnPoint = spawnPoint;
    setPosition(spawnPoint);
    m_velocity = QPointF(0, 0);
    m_health = 100;
    m_onGround = false;
    setState(State::IDLE);
    emit healthChanged(m_health);
    emit respawned();
}

void Player2D::addScore(int points) {
    m_score += points;
    emit scoreChanged(m_score);
}

void Player2D::collectCoin() {
    m_coins++;
    addScore(100);
    emit coinsChanged(m_coins);
}
