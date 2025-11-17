#include "Enemy.h"
#include <QDebug>
#include <cmath>

Enemy::Enemy(Type type, const QPointF& position, int riddleId, QObject* parent)
    : QObject(parent)
    , m_type(type)
    , m_position(position)
    , m_state(State::IDLE)
    , m_facingRight(false)
    , m_riddleId(riddleId)
    , m_riddleTriggered(false)
    , m_patrolDistance(64.0f)
    , m_startPosition(position)
    , m_moveSpeed(30.0f)
{
    loadAnimations();
}

Enemy::~Enemy() {
    // Clean up animations
    for (auto* sprite : m_animations.values()) {
        delete sprite;
    }
}

void Enemy::loadAnimations() {
    QString basePath = "assets/sprites/";
    QString prefix = (m_type == Type::PINK_MONSTER) ? "Pink_Monster_" : "Owlet_Monster_";
    
    // Idle animation (4 frames)
    AnimatedSprite* idleSprite = new AnimatedSprite();
    if (idleSprite->loadSpriteSheet(basePath + prefix + "Idle_4.png", 32, 32, 4)) {
        idleSprite->setFrameRate(8.0f);
        idleSprite->setLoop(true);
        idleSprite->play();
        m_animations[State::IDLE] = idleSprite;
    } else {
        qDebug() << "Failed to load enemy idle animation:" << prefix;
        delete idleSprite;
    }
    
    // Walk animation (6 frames)
    AnimatedSprite* walkSprite = new AnimatedSprite();
    if (walkSprite->loadSpriteSheet(basePath + prefix + "Walk_6.png", 32, 32, 6)) {
        walkSprite->setFrameRate(12.0f);
        walkSprite->setLoop(true);
        walkSprite->play();
        m_animations[State::WALKING] = walkSprite;
    } else {
        qDebug() << "Failed to load enemy walk animation:" << prefix;
        delete walkSprite;
    }
    
    // Hurt animation (4 frames)
    AnimatedSprite* hurtSprite = new AnimatedSprite();
    if (hurtSprite->loadSpriteSheet(basePath + prefix + "Hurt_4.png", 32, 32, 4)) {
        hurtSprite->setFrameRate(12.0f);
        hurtSprite->setLoop(false);
        m_animations[State::HURT] = hurtSprite;
    } else {
        qDebug() << "Failed to load enemy hurt animation:" << prefix;
        delete hurtSprite;
    }
    
    // Death animation (8 frames)
    AnimatedSprite* deathSprite = new AnimatedSprite();
    if (deathSprite->loadSpriteSheet(basePath + prefix + "Death_8.png", 32, 32, 8)) {
        deathSprite->setFrameRate(12.0f);
        deathSprite->setLoop(false);
        m_animations[State::DEAD] = deathSprite;
    } else {
        qDebug() << "Failed to load enemy death animation:" << prefix;
        delete deathSprite;
    }
    
    // Start with idle animation
    setState(State::IDLE);
}

void Enemy::setPosition(const QPointF& pos) {
    m_position = pos;
}

QRectF Enemy::boundingBox() const {
    return QRectF(m_position.x(), m_position.y(), WIDTH, HEIGHT);
}

void Enemy::setState(State state) {
    if (m_state != state) {
        m_state = state;
        // Reset animation when state changes
        if (m_animations.contains(state)) {
            m_animations[state]->reset();
            m_animations[state]->play();
        }
    }
}

bool Enemy::isDeathAnimationFinished() const {
    if (m_state == State::DEAD && m_animations.contains(State::DEAD)) {
        return m_animations[State::DEAD]->isFinished();
    }
    return false;
}

void Enemy::update(float deltaTime) {
    // Update animation
    updateAnimation(deltaTime);
    
    if (m_state == State::DEAD) {
        // Check if death animation is complete
        if (isDeathAnimationFinished()) {
            emit deathAnimationComplete();
        }
        return;
    }
    
    // Patrol behavior (walk back and forth)
    if (m_state != State::HURT) {
        patrol();
    }
}

void Enemy::updateAnimation(float deltaTime) {
    if (m_animations.contains(m_state)) {
        m_animations[m_state]->update(deltaTime);
    }
}

void Enemy::patrol() {
    // Simple patrol: walk left and right around spawn point
    setState(State::WALKING);
    
    float distance = m_position.x() - m_startPosition.x();
    
    if (m_facingRight) {
        m_position.setX(m_position.x() + m_moveSpeed * (1.0f / 60.0f));
        if (distance > m_patrolDistance) {
            m_facingRight = false;
        }
    } else {
        m_position.setX(m_position.x() - m_moveSpeed * (1.0f / 60.0f));
        if (distance < -m_patrolDistance) {
            m_facingRight = true;
        }
    }
}

void Enemy::takeDamage() {
    if (m_state != State::DEAD && m_state != State::HURT) {
        setState(State::HURT);
    }
}

void Enemy::die() {
    setState(State::DEAD);
    // Ensure death animation plays
    if (m_animations.contains(State::DEAD)) {
        m_animations[State::DEAD]->reset();
        m_animations[State::DEAD]->play();
    }
    emit died();
}

