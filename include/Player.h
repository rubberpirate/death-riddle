#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QObject>

class Player : public QObject {
    Q_OBJECT

public:
    enum class State {
        IDLE,
        THINKING,
        DAMAGED,
        CELEBRATING,
        DEFEATED
    };

    explicit Player(QObject* parent = nullptr);
    virtual ~Player() = default;

    // Health management
    int getHealth() const { return m_health; }
    int getMaxHealth() const { return m_maxHealth; }
    void setHealth(int health);
    void takeDamage(int damage);
    void heal(int amount);
    bool isAlive() const { return m_health > 0; }

    // Player state
    State getState() const { return m_state; }
    void setState(State state);

    // Score and progress
    int getScore() const { return m_score; }
    void addScore(int points);
    int getRiddlesSolved() const { return m_riddlesSolved; }
    void incrementRiddlesSolved();
    int getFailedAttempts() const { return m_failedAttempts; }
    void incrementFailedAttempts();

    // Player info
    QString getName() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

    // Reset player state
    void reset();

signals:
    void healthChanged(int newHealth, int maxHealth);
    void stateChanged(State newState);
    void scoreChanged(int newScore);
    void playerDied();
    void playerHealed(int amount);
    void playerDamaged(int damage);

private:
    int m_health;
    int m_maxHealth;
    int m_score;
    int m_riddlesSolved;
    int m_failedAttempts;
    State m_state;
    QString m_name;
};

#endif // PLAYER_H
