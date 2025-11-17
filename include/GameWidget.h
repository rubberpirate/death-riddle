#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "Player2D.h"
#include "Level.h"
#include "Riddle.h"
#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>
#include <QSet>
#include <QKeyEvent>

class GameWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameWidget(QWidget* parent = nullptr);
    virtual ~GameWidget() = default;
    
    void startGame();
    void pauseGame();
    void resumeGame();
    void resetGame();

signals:
    void returnToMainMenu();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void timerEvent(QTimerEvent* event) override;

private slots:
    void update();
    void onRiddleSolved(bool success);

private:
    void setupGame();
    void loadLevel(int levelNumber);
    void updatePhysics(float deltaTime);
    void checkCollisions();
    void checkTileInteractions();
    void handleInput();
    
    // Rendering
    void drawLevel(QPainter& painter);
    void drawPlayer(QPainter& painter);
    void drawEnemies(QPainter& painter);
    void drawVictoryScreen(QPainter& painter);
    void drawEnemy(QPainter& painter, class Enemy* enemy);
    void drawUI(QPainter& painter);
    void drawTile(QPainter& painter, const Tile* tile);
    void drawRetryScreen(QPainter& painter);
    
    // Riddle system
    void showRiddle(int riddleId, class Enemy* enemy);
    void hideRiddle();
    void checkEnemyCollisions();
    
    // Game state
    Player2D* m_player;
    Level* m_currentLevel;
    QVector<Riddle*> m_riddles;
    Riddle* m_activeRiddle;
    class Enemy* m_activeEnemy;
    
    // Timing
    QTimer* m_gameTimer;
    QElapsedTimer m_elapsedTimer;
    float m_lastFrameTime;
    
    // Input
    QSet<int> m_pressedKeys;
    bool m_gamePaused;
    bool m_riddleActive;
    bool m_showRetryScreen;
    bool m_showVictoryScreen;
    
    // Score tracking
    int m_highestScore;
    
    // Game progress
    bool m_hasKey;
    bool m_topDownMode;  // For Level 6 maze
    
    // UI elements
    QLabel* m_healthLabel;
    QLabel* m_scoreLabel;
    QLabel* m_livesLabel;
    QLabel* m_levelLabel;
    QWidget* m_riddleWidget;
    
    // Constants
    static constexpr int TARGET_FPS = 60;
    static constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
};

#endif // GAMEWIDGET_H
