#ifndef GAMEMASTER_H
#define GAMEMASTER_H

#include <QObject>
#include <QString>
#include <QTimer>

class GameMaster : public QObject {
    Q_OBJECT

public:
    enum class Mood {
        NEUTRAL,
        AMUSED,
        IMPRESSED,
        ANGRY,
        MENACING,
        SATISFIED
    };

    explicit GameMaster(QObject* parent = nullptr);
    virtual ~GameMaster() = default;

    // Dialogue and reactions
    QString getCurrentDialogue() const { return m_currentDialogue; }
    void setDialogue(const QString& dialogue);
    
    Mood getMood() const { return m_mood; }
    void setMood(Mood mood);

    // Reactions to player actions
    QString reactToCorrectAnswer();
    QString reactToWrongAnswer();
    QString reactToLowHealth();
    QString reactToHighScore();

    // Animation state
    bool isAnimating() const { return m_isAnimating; }
    void startAnimation();
    void stopAnimation();

signals:
    void dialogueChanged(QString dialogue);
    void moodChanged(Mood mood);
    void animationStateChanged(bool animating);

private:
    QString m_currentDialogue;
    Mood m_mood;
    bool m_isAnimating;
};

#endif // GAMEMASTER_H
