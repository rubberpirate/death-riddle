#include "Player.h"
#include <algorithm>

Player::Player(QObject* parent)
    : QObject(parent)
    , m_health(100)
    , m_maxHealth(100)
    , m_score(0)
    , m_riddlesSolved(0)
    , m_failedAttempts(0)
    , m_state(State::IDLE)
    , m_name("Player")
{
}

void Player::setHealth(int health) {
    int oldHealth = m_health;
    m_health = std::clamp(health, 0, m_maxHealth);
    
    if (m_health != oldHealth) {
        emit healthChanged(m_health, m_maxHealth);
        
        if (m_health <= 0 && oldHealth > 0) {
            setState(State::DEFEATED);
            emit playerDied();
        }
    }
}

void Player::takeDamage(int damage) {
    if (damage > 0 && m_health > 0) {
        setState(State::DAMAGED);
        setHealth(m_health - damage);
        emit playerDamaged(damage);
    }
}

void Player::heal(int amount) {
    if (amount > 0 && m_health > 0) {
        setHealth(m_health + amount);
        emit playerHealed(amount);
    }
}

void Player::setState(State state) {
    if (m_state != state) {
        m_state = state;
        emit stateChanged(state);
    }
}

void Player::addScore(int points) {
    m_score += points;
    emit scoreChanged(m_score);
}

void Player::incrementRiddlesSolved() {
    m_riddlesSolved++;
}

void Player::incrementFailedAttempts() {
    m_failedAttempts++;
}

void Player::reset() {
    m_health = m_maxHealth;
    m_score = 0;
    m_riddlesSolved = 0;
    m_failedAttempts = 0;
    m_state = State::IDLE;
    emit healthChanged(m_health, m_maxHealth);
    emit scoreChanged(m_score);
    emit stateChanged(m_state);
}
