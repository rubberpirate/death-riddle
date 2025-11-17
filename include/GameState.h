#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Player.h"
#include "GameMaster.h"
#include "Story.h"
#include <QObject>

class GameState : public QObject {
    Q_OBJECT

public:
    enum class State {
        MENU,
        INTRO,
        PLAYING,
        RIDDLE_ACTIVE,
        DIALOGUE,
        GAME_OVER,
        ENDING
    };

    explicit GameState(QObject* parent = nullptr);
    virtual ~GameState() = default;

    // Game flow
    void startNewGame();
    void continueGame();
    void pauseGame();
    void resumeGame();
    void endGame();

    // State management
    State getCurrentState() const { return m_state; }
    void setState(State state);

    // Component access
    Player* getPlayer() const { return m_player; }
    GameMaster* getGameMaster() const { return m_gameMaster; }
    Story* getStory() const { return m_story; }

    // Riddle handling
    void submitAnswer(const QString& answer);
    void requestHint();
    int getHintsRemaining() const { return m_hintsRemaining; }

    // Game logic
    void processChoice(int choiceIndex);
    void checkGameOver();
    void checkEnding();

    bool isPaused() const { return m_paused; }

signals:
    void stateChanged(State newState);
    void gameStarted();
    void gameOver(Story::Ending ending);
    void riddleResult(bool correct, QString feedback);
    void hintProvided(QString hint);
    void narrativeUpdate(QString text);
    void gameMasterSpeaks(QString dialogue);

private slots:
    void onPlayerDied();
    void onStoryEnding(Story::Ending ending);
    void onHealthChanged(int health, int maxHealth);

private:
    void connectSignals();

    Player* m_player;
    GameMaster* m_gameMaster;
    Story* m_story;
    State m_state;
    bool m_paused;
    int m_hintsRemaining;
    int m_currentAttempts;
};

#endif // GAMESTATE_H
