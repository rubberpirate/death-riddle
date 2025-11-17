#include "GameWidget.h"
#include "Enemy.h"
#include <QPainter>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QPushButton>
#include <cmath>

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent)
    , m_player(new Player2D(this))
    , m_currentLevel(nullptr)
    , m_activeRiddle(nullptr)
    , m_activeEnemy(nullptr)
    , m_gameTimer(new QTimer(this))
    , m_lastFrameTime(0)
    , m_gamePaused(false)
    , m_riddleActive(false)
    , m_showRetryScreen(false)
    , m_showVictoryScreen(false)
    , m_highestScore(0)
    , m_hasKey(false)
    , m_topDownMode(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(960, 640);
    
    setupGame();
    
    // Setup game timer
    connect(m_gameTimer, &QTimer::timeout, this, &GameWidget::update);
    m_gameTimer->setInterval(1000 / TARGET_FPS);
    
    // Create riddles
    for (int i = 0; i < 5; ++i) {
        m_riddles.append(new Riddle(this));
    }
    
    // Riddle 1
    m_riddles[0]->setQuestion("What has keys but no locks, space but no room, and you can enter but can't go inside?");
    m_riddles[0]->setAnswer("keyboard");
    m_riddles[0]->setHint("It's something you're using right now...");
    
    // Riddle 2
    m_riddles[1]->setQuestion("Decode: GUVF VF ABG SERR (ROT13)");
    m_riddles[1]->setAnswer("THIS IS NOT FREE");
    m_riddles[1]->setHint("Shift each letter by 13");
    
    // Riddle 3
    m_riddles[2]->setQuestion("Three gates: Gold says 'Freedom lies behind Silver', Silver says 'Freedom is not here', Bronze says 'Gold speaks truth'. Only ONE speaks truth. Which gate?");
    m_riddles[2]->setAnswer("gold");
    m_riddles[2]->setHint("If Bronze is true, then Gold is true - but only one can be true...");
    
    // Riddle 4
    m_riddles[3]->setQuestion("Convert binary to text: 01000110 01010010 01000101 01000101");
    m_riddles[3]->setAnswer("FREE");
    m_riddles[3]->setHint("8 bits = 1 ASCII character");
    
    // Riddle 5
    m_riddles[4]->setQuestion("I am the beginning of eternity, the end of time and space. I am the beginning of every end, the end of every place. What am I?");
    m_riddles[4]->setAnswer("e");
    m_riddles[4]->setHint("Look at the first and last letters of the words...");
}

void GameWidget::setupGame() {
    // Create UI labels
    m_healthLabel = new QLabel("HP: 100", this);
    m_scoreLabel = new QLabel("Score: 0", this);
    m_livesLabel = new QLabel("Lives: 3", this);
    m_levelLabel = new QLabel("Level 1", this);
    
    QString labelStyle = "QLabel { background-color: rgba(0,0,0,180); color: white; "
                        "padding: 8px; border-radius: 4px; font-size: 14px; font-weight: bold; }";
    
    m_healthLabel->setStyleSheet(labelStyle);
    m_scoreLabel->setStyleSheet(labelStyle);
    m_livesLabel->setStyleSheet(labelStyle);
    m_levelLabel->setStyleSheet(labelStyle + "font-size: 16px;");
    
    m_healthLabel->setGeometry(10, 10, 120, 30);
    m_scoreLabel->setGeometry(140, 10, 150, 30);
    m_livesLabel->setGeometry(300, 10, 100, 30);
    m_levelLabel->setGeometry(410, 10, 200, 30);
    
    // Connect player signals
    connect(m_player, &Player2D::healthChanged, this, [this](int health) {
        m_healthLabel->setText(QString("HP: %1").arg(health));
    });
    
    connect(m_player, &Player2D::scoreChanged, this, [this](int score) {
        m_scoreLabel->setText(QString("Score: %1").arg(score));
    });
    
    connect(m_player, &Player2D::livesChanged, this, [this](int lives) {
        m_livesLabel->setText(QString("Lives: %1").arg(lives));
    });
    
    connect(m_player, &Player2D::died, this, [this]() {
        if (m_player->lives() > 0) {
            QTimer::singleShot(1000, this, [this]() {
                m_player->respawn(m_currentLevel->spawnPoint());
            });
        } else {
            // Update highest score
            if (m_player->score() > m_highestScore) {
                m_highestScore = m_player->score();
            }
            
            // Show retry screen
            pauseGame();
            m_showRetryScreen = true;
            QWidget::update();  // Force repaint to show retry screen
        }
    });
}

void GameWidget::startGame() {
    m_player->respawn(QPointF(64, 500));
    m_player->addScore(-m_player->score());  // Reset score
    loadLevel(1);
    m_gameTimer->start();
    m_elapsedTimer.start();
    m_lastFrameTime = 0;
    m_gamePaused = false;
    setFocus();
}

void GameWidget::pauseGame() {
    m_gamePaused = true;
    m_gameTimer->stop();
    
    // Clear all pressed keys when pausing
    m_pressedKeys.clear();
    
    // Stop player movement
    if (m_player) {
        m_player->setVelocity(QPointF(0, m_player->velocity().y()));
    }
}

void GameWidget::resumeGame() {
    m_gamePaused = false;
    
    // Clear pressed keys again when resuming
    m_pressedKeys.clear();
    
    // Restart elapsed timer and reset last frame time to prevent huge delta time spike
    m_elapsedTimer.restart();
    m_lastFrameTime = 0;
    m_gameTimer->start();
}

void GameWidget::resetGame() {
    startGame();
}

void GameWidget::loadLevel(int levelNumber) {
    if (m_currentLevel) {
        delete m_currentLevel;
    }
    
    // Reset key status and set top-down mode for Level 6
    if (levelNumber == 6) {
        m_hasKey = false;
        m_topDownMode = true;
    } else {
        m_topDownMode = false;
    }
    
    m_currentLevel = new Level(levelNumber, this);
    m_levelLabel->setText(QString("Level %1: %2").arg(levelNumber).arg(m_currentLevel->name()));
    m_player->setPosition(m_currentLevel->spawnPoint());
    
    // Connect level signals
    connect(m_currentLevel, &Level::riddleTriggered, this, &GameWidget::showRiddle);
    connect(m_currentLevel, &Level::levelComplete, this, [this]() {
        pauseGame();
        
        if (m_currentLevel->levelNumber() < 6) {
            QMessageBox::information(this, "Level Complete!", 
                QString("You completed %1!\n\nScore: %2").arg(m_currentLevel->name()).arg(m_player->score()));
            loadLevel(m_currentLevel->levelNumber() + 1);
            resumeGame();
        } else {
            // Game complete - show victory screen
            m_showVictoryScreen = true;
            update();
        }
    });
}

void GameWidget::update() {
    if (m_gamePaused || m_riddleActive || m_showVictoryScreen) {
        QWidget::update();
        return;
    }
    
    // Calculate delta time
    float currentTime = m_elapsedTimer.elapsed() / 1000.0f;
    float deltaTime = currentTime - m_lastFrameTime;
    m_lastFrameTime = currentTime;
    
    // Clamp delta time
    if (deltaTime > 0.1f) deltaTime = 0.1f;
    
    handleInput();
    updatePhysics(deltaTime);
    checkCollisions();
    checkTileInteractions();
    checkEnemyCollisions();
    
    // Update enemies
    if (m_currentLevel) {
        for (Enemy* enemy : m_currentLevel->enemies()) {
            if (enemy && !enemy->isDead()) {
                enemy->update(deltaTime);
            }
        }
        
        // Remove dead enemies after their death animation finishes
        m_currentLevel->enemies().erase(
            std::remove_if(m_currentLevel->enemies().begin(), m_currentLevel->enemies().end(),
                [](Enemy* enemy) {
                    if (enemy && enemy->isDead() && enemy->isDeathAnimationFinished()) {
                        qDebug() << "Removing dead enemy after animation finished";
                        delete enemy;
                        return true;
                    }
                    return false;
                }),
            m_currentLevel->enemies().end()
        );
    }
    
    QWidget::update();
}

void GameWidget::handleInput() {
    // Don't process input if game is paused or riddle is active
    if (m_gamePaused || m_riddleActive) {
        return;
    }
    
    if (m_topDownMode) {
        // Top-down movement (4 directions, no jumping)
        bool movingLeft = m_pressedKeys.contains(Qt::Key_Left) || m_pressedKeys.contains(Qt::Key_A);
        bool movingRight = m_pressedKeys.contains(Qt::Key_Right) || m_pressedKeys.contains(Qt::Key_D);
        bool movingUp = m_pressedKeys.contains(Qt::Key_Up) || m_pressedKeys.contains(Qt::Key_W);
        bool movingDown = m_pressedKeys.contains(Qt::Key_Down) || m_pressedKeys.contains(Qt::Key_S);
        
        QPointF velocity = m_player->velocity();
        float speed = 150.0f;  // Constant speed for top-down
        
        // Reset velocity
        velocity.setX(0);
        velocity.setY(0);
        
        // Set velocity based on input
        if (movingLeft && !movingRight) {
            velocity.setX(-speed);
        } else if (movingRight && !movingLeft) {
            velocity.setX(speed);
        }
        
        if (movingUp && !movingDown) {
            velocity.setY(-speed);
        } else if (movingDown && !movingUp) {
            velocity.setY(speed);
        }
        
        m_player->setVelocity(velocity);
    } else {
        // Platformer movement (left/right + jumping)
        bool movingLeft = m_pressedKeys.contains(Qt::Key_Left) || m_pressedKeys.contains(Qt::Key_A);
        bool movingRight = m_pressedKeys.contains(Qt::Key_Right) || m_pressedKeys.contains(Qt::Key_D);
        bool jumping = m_pressedKeys.contains(Qt::Key_Up) || m_pressedKeys.contains(Qt::Key_W) || 
                       m_pressedKeys.contains(Qt::Key_Space);
        
        if (movingLeft && !movingRight) {
            m_player->moveLeft();
        } else if (movingRight && !movingLeft) {
            m_player->moveRight();
        } else {
            m_player->stopHorizontalMovement();
        }
        
        if (jumping) {
            m_player->jump();
        }
    }
}

void GameWidget::updatePhysics(float deltaTime) {
    // Apply gravity only in platformer mode (not in top-down maze)
    if (!m_topDownMode) {
        m_player->applyGravity(deltaTime);
    }
    
    // Update player position
    m_player->update(deltaTime);
}

void GameWidget::checkCollisions() {
    QRectF playerBox = m_player->boundingBox();
    QVector<Tile*> nearbyTiles = m_currentLevel->getTilesInArea(playerBox.adjusted(-10, -10, 10, 10));
    
    bool onGround = false;
    
    for (Tile* tile : nearbyTiles) {
        if (tile->type != TileType::SOLID && tile->type != TileType::MOVING_PLATFORM) {
            continue;
        }
        
        if (!playerBox.intersects(tile->boundingBox)) {
            continue;
        }
        
        // Calculate overlap
        float overlapLeft = playerBox.right() - tile->boundingBox.left();
        float overlapRight = tile->boundingBox.right() - playerBox.left();
        float overlapTop = playerBox.bottom() - tile->boundingBox.top();
        float overlapBottom = tile->boundingBox.bottom() - playerBox.top();
        
        float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});
        
        QPointF pos = m_player->position();
        QPointF vel = m_player->velocity();
        
        if (m_topDownMode) {
            // Top-down collision - treat all walls equally
            if (minOverlap == overlapTop) {
                pos.setY(tile->boundingBox.top() - Player2D::HEIGHT);
                vel.setY(0);
            } else if (minOverlap == overlapBottom) {
                pos.setY(tile->boundingBox.bottom());
                vel.setY(0);
            } else if (minOverlap == overlapLeft) {
                pos.setX(tile->boundingBox.left() - Player2D::WIDTH);
                vel.setX(0);
            } else if (minOverlap == overlapRight) {
                pos.setX(tile->boundingBox.right());
                vel.setX(0);
            }
        } else {
            // Platformer collision - different behavior for top/bottom vs sides
            if (minOverlap == overlapTop && vel.y() > 0) {
                // Collision from top (landing on platform)
                pos.setY(tile->boundingBox.top() - Player2D::HEIGHT);
                vel.setY(0);
                onGround = true;
            } else if (minOverlap == overlapBottom && vel.y() < 0) {
                // Collision from bottom (hitting ceiling)
                pos.setY(tile->boundingBox.bottom());
                vel.setY(0);
            } else if (minOverlap == overlapLeft) {
                // Collision from left
                pos.setX(tile->boundingBox.left() - Player2D::WIDTH);
                vel.setX(0);
            } else if (minOverlap == overlapRight) {
                // Collision from right
                pos.setX(tile->boundingBox.right());
                vel.setX(0);
            }
        }
        
        m_player->setPosition(pos);
        m_player->setVelocity(vel);
    }
    
    // Only update onGround state in platformer mode
    if (!m_topDownMode) {
        m_player->setOnGround(onGround);
    }
}

void GameWidget::checkTileInteractions() {
    QRectF playerBox = m_player->boundingBox();
    QVector<Tile*> tiles = m_currentLevel->getTilesInArea(playerBox);
    
    for (Tile* tile : tiles) {
        if (!playerBox.intersects(tile->boundingBox)) continue;
        
        switch (tile->type) {
            case TileType::COIN:
                if (!tile->collected) {
                    m_currentLevel->collectCoin(tile);
                    m_player->collectCoin();
                }
                break;
            
            case TileType::KEY:
                if (!tile->collected) {
                    tile->collected = true;
                    m_hasKey = true;
                    m_player->addScore(1000);
                    QMessageBox::information(this, "Key Found!", 
                        "You found the key! Now you can reach the goal!\n\n+1000 points");
                }
                break;
                
            case TileType::SPIKE:
                m_player->takeDamage(25);
                m_player->setPosition(m_currentLevel->spawnPoint());
                break;
                
            case TileType::RIDDLE_TRIGGER:
                // Old riddle trigger system - now using enemies
                break;
                
            case TileType::GOAL:
                // Check if player has key for Level 6
                if (m_currentLevel && m_currentLevel->levelNumber() == 6 && !m_hasKey) {
                    QMessageBox::warning(this, "Locked!", 
                        "You need to find the key first!");
                } else {
                    m_currentLevel->setComplete(true);
                    emit m_currentLevel->levelComplete();
                }
                break;
                
            case TileType::CHECKPOINT:
                m_currentLevel->activateCheckpoint(tile);
                break;
                
            default:
                break;
        }
    }
}

void GameWidget::checkEnemyCollisions() {
    if (!m_currentLevel || m_riddleActive) return;
    
    QRectF playerBox = m_player->boundingBox();
    
    for (Enemy* enemy : m_currentLevel->enemies()) {
        if (!enemy || enemy->isDead() || enemy->isRiddleTriggered()) continue;
        
        QRectF enemyBox = enemy->boundingBox();
        if (playerBox.intersects(enemyBox)) {
            // Mark enemy riddle as triggered to prevent re-trigger
            enemy->setRiddleTriggered(true);
            // Trigger riddle for this enemy
            showRiddle(enemy->riddleId(), enemy);
            break;  // Only trigger one riddle at a time
        }
    }
}

void GameWidget::showRiddle(int riddleId, Enemy* enemy) {
    if (riddleId < 0 || riddleId >= m_riddles.size()) return;
    
    pauseGame();
    
    // Clear all pressed keys to prevent input carry-over
    m_pressedKeys.clear();
    
    // Stop player movement
    m_player->setVelocity(QPointF(0, m_player->velocity().y()));
    
    m_riddleActive = true;
    m_activeRiddle = m_riddles[riddleId];
    m_activeEnemy = enemy;
    
    bool ok;
    QString answer = QInputDialog::getText(this, "Riddle Challenge", 
        m_activeRiddle->getQuestion() + "\n\nHint: " + m_activeRiddle->getHint(),
        QLineEdit::Normal, "", &ok);
    
    if (ok) {
        onRiddleSolved(m_activeRiddle->checkAnswerCaseInsensitive(answer));
    } else {
        m_riddleActive = false;
        resumeGame();
    }
}

void GameWidget::hideRiddle() {
    m_riddleActive = false;
    m_activeRiddle = nullptr;
}

void GameWidget::onRiddleSolved(bool success) {
    if (success) {
        QMessageBox::information(this, "Correct!", 
            "Well done! The Game Master is impressed.\n\n+500 points");
        m_player->addScore(500);
        m_player->heal(25);
        
        // Kill the enemy with death animation
        if (m_activeEnemy) {
            qDebug() << "Triggering enemy death animation";
            m_activeEnemy->die();
        }
    } else {
        QMessageBox::warning(this, "Incorrect!", 
            "Wrong answer! The Game Master is not pleased.\n\n-25 HP");
        m_player->takeDamage(25);
        
        // Enemy takes damage but doesn't die
        if (m_activeEnemy) {
            m_activeEnemy->takeDamage();
        }
    }
    
    hideRiddle();
    
    // Clear pressed keys again after dialog boxes
    m_pressedKeys.clear();
    
    // Ensure player is stopped
    m_player->setVelocity(QPointF(0, m_player->velocity().y()));
    
    // Clear active enemy reference
    m_activeEnemy = nullptr;
    
    resumeGame();
}

void GameWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);  // Pixelated style
    
    // Background
    painter.fillRect(rect(), QColor(25, 25, 40));
    
    // Camera offset (center on player)
    int cameraX = std::max(0, std::min(
        static_cast<int>(m_player->position().x() - width() / 2),
        m_currentLevel->width() * Level::TILE_SIZE - width()
    ));
    
    painter.translate(-cameraX, 0);
    
    if (m_currentLevel) {
        drawLevel(painter);
        drawEnemies(painter);
    }
    
    drawPlayer(painter);
    
    painter.translate(cameraX, 0);
    
    // UI is always on screen
    drawUI(painter);
    
    // Victory screen (drawn last, over everything)
    if (m_showVictoryScreen) {
        drawVictoryScreen(painter);
    }
    // Retry screen (drawn last, over everything)
    else if (m_showRetryScreen) {
        drawRetryScreen(painter);
    }
}

void GameWidget::drawLevel(QPainter& painter) {
    for (int y = 0; y < m_currentLevel->height(); ++y) {
        for (int x = 0; x < m_currentLevel->width(); ++x) {
            const Tile* tile = m_currentLevel->getTileAt(x, y);
            if (tile && tile->type != TileType::EMPTY) {
                drawTile(painter, tile);
            }
        }
    }
}

void GameWidget::drawTile(QPainter& painter, const Tile* tile) {
    QRectF rect = tile->boundingBox;
    
    switch (tile->type) {
        case TileType::SOLID:
            painter.fillRect(rect, QColor(100, 100, 100));
            painter.setPen(QColor(70, 70, 70));
            painter.drawRect(rect);
            break;
            
        case TileType::SPIKE:
            painter.fillRect(rect, QColor(200, 50, 50));
            // Draw spikes
            for (int i = 0; i < 4; ++i) {
                QPointF p1(rect.left() + i * 8, rect.bottom());
                QPointF p2(rect.left() + i * 8 + 4, rect.top());
                QPointF p3(rect.left() + i * 8 + 8, rect.bottom());
                painter.drawLine(p1, p2);
                painter.drawLine(p2, p3);
            }
            break;
            
        case TileType::COIN:
            if (!tile->collected) {
                painter.setBrush(QColor(255, 215, 0));
                painter.setPen(QColor(200, 165, 0));
                painter.drawEllipse(rect.center(), 8, 8);
            }
            break;
            
        case TileType::RIDDLE_TRIGGER: {
            if (!tile->activated) {
                painter.fillRect(rect, QColor(150, 50, 200));
                painter.setPen(Qt::white);
                QFont font = painter.font();
                font.setPointSize(20);
                painter.setFont(font);
                painter.drawText(rect, Qt::AlignCenter, "?");
            }
            break;
        }
            
        case TileType::KEY:
            if (!tile->collected) {
                painter.setBrush(QColor(255, 215, 0));
                painter.setPen(QColor(150, 100, 0));
                // Draw key shape
                painter.drawEllipse(rect.center().x() - 6, rect.center().y() - 4, 8, 8);
                painter.drawRect(rect.center().x() + 2, rect.center().y() - 2, 8, 4);
                painter.drawRect(rect.center().x() + 7, rect.center().y() - 4, 2, 2);
                painter.drawRect(rect.center().x() + 7, rect.center().y() + 2, 2, 2);
            }
            break;
            
        case TileType::GOAL: {
            painter.fillRect(rect, QColor(50, 255, 50));
            painter.setPen(Qt::white);
            QFont font = painter.font();
            font.setPointSize(20);
            painter.setFont(font);
            painter.drawText(rect, Qt::AlignCenter, "★");
            break;
        }
            
        case TileType::CHECKPOINT: {
            painter.fillRect(rect, QColor(100, 150, 255));
            if (tile->activated) {
                painter.fillRect(rect, QColor(100, 255, 150));
            }
            break;
        }
            
        default:
            break;
    }
}

void GameWidget::drawPlayer(QPainter& painter) {
    AnimatedSprite* sprite = m_player->getCurrentSprite();
    if (!sprite) {
        // Fallback to simple rectangle if no sprite loaded
        QRectF box = m_player->boundingBox();
        QColor bodyColor(70, 130, 180);
        if (m_player->state() == Player2D::State::DEAD) {
            bodyColor = QColor(100, 100, 100);
        }
        painter.fillRect(box, bodyColor);
        return;
    }
    
    QRectF box = m_player->boundingBox();
    const QPixmap& spriteSheet = sprite->getPixmap();
    QRect sourceRect = sprite->getCurrentFrame();
    
    // Flip horizontally if facing left
    if (!m_player->isFacingRight()) {
        painter.save();
        painter.translate(box.x() + box.width(), box.y());
        painter.scale(-1, 1);
        painter.drawPixmap(QRect(0, 0, box.width(), box.height()), 
                          spriteSheet, sourceRect);
        painter.restore();
    } else {
        painter.drawPixmap(box.toRect(), spriteSheet, sourceRect);
    }
}

void GameWidget::drawEnemies(QPainter& painter) {
    if (!m_currentLevel) return;
    
    for (Enemy* enemy : m_currentLevel->enemies()) {
        if (enemy) {
            drawEnemy(painter, enemy);
        }
    }
}

void GameWidget::drawEnemy(QPainter& painter, Enemy* enemy) {
    AnimatedSprite* sprite = enemy->getCurrentSprite();
    if (!sprite) {
        // Fallback to simple rectangle
        QRectF box = enemy->boundingBox();
        painter.fillRect(box, QColor(255, 100, 150));
        return;
    }
    
    QRectF box = enemy->boundingBox();
    const QPixmap& spriteSheet = sprite->getPixmap();
    QRect sourceRect = sprite->getCurrentFrame();
    
    // Flip horizontally based on facing direction
    if (!enemy->isFacingRight()) {
        painter.save();
        painter.translate(box.x() + box.width(), box.y());
        painter.scale(-1, 1);
        painter.drawPixmap(QRect(0, 0, box.width(), box.height()), 
                          spriteSheet, sourceRect);
        painter.restore();
    } else {
        painter.drawPixmap(box.toRect(), spriteSheet, sourceRect);
    }
}

void GameWidget::drawUI(QPainter& painter) {
    // UI labels are already QWidgets, they paint themselves
}

void GameWidget::drawRetryScreen(QPainter& painter) {
    // Semi-transparent dark overlay
    painter.fillRect(rect(), QColor(0, 0, 0, 200));
    
    // Game Over title
    painter.setPen(QColor(255, 50, 50));
    QFont titleFont;
    titleFont.setPointSize(48);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignTop, "GAME OVER");
    
    // Scores
    painter.setPen(Qt::white);
    QFont scoreFont;
    scoreFont.setPointSize(24);
    painter.setFont(scoreFont);
    
    int centerY = height() / 2 - 50;
    painter.drawText(QRect(0, centerY, width(), 40), Qt::AlignCenter, 
                    QString("Current Score: %1").arg(m_player->score()));
    painter.drawText(QRect(0, centerY + 50, width(), 40), Qt::AlignCenter, 
                    QString("Highest Score: %1").arg(m_highestScore));
    
    // Instructions
    QFont instFont;
    instFont.setPointSize(18);
    painter.setFont(instFont);
    painter.setPen(QColor(200, 200, 200));
    painter.drawText(QRect(0, height() - 100, width(), 40), Qt::AlignCenter, 
                    "Press R to Retry or ESC to Return to Menu");
}

void GameWidget::drawVictoryScreen(QPainter& painter) {
    // Victory background with golden gradient
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0, QColor(255, 215, 0, 230));      // Gold
    gradient.setColorAt(0.5, QColor(255, 140, 0, 230));    // Dark Orange
    gradient.setColorAt(1, QColor(184, 134, 11, 230));     // Dark Golden
    painter.fillRect(rect(), gradient);
    
    // Victory title with glow effect
    painter.setPen(QColor(255, 255, 255));
    QFont titleFont;
    titleFont.setPointSize(56);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    
    // Draw shadow for glow effect
    painter.setPen(QColor(0, 0, 0, 100));
    painter.drawText(QRect(2, 52, width(), 80), Qt::AlignCenter, "VICTORY!");
    
    // Draw main title
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(QRect(0, 50, width(), 80), Qt::AlignCenter, "VICTORY!");
    
    // Congratulations message
    painter.setPen(QColor(255, 255, 255));
    QFont messageFont;
    messageFont.setPointSize(28);
    messageFont.setBold(true);
    painter.setFont(messageFont);
    painter.drawText(QRect(0, 150, width(), 40), Qt::AlignCenter, 
                    "You Escaped from the Game Master!");
    
    // Subtext
    QFont subFont;
    subFont.setPointSize(20);
    painter.setFont(subFont);
    painter.setPen(QColor(240, 240, 240));
    painter.drawText(QRect(0, 200, width(), 40), Qt::AlignCenter, 
                    "You are truly free!");
    
    // Draw decorative stars
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 255, 255, 200));
    QPolygonF star;
    auto drawStar = [&](int x, int y, int size) {
        star.clear();
        for (int i = 0; i < 10; ++i) {
            float angle = i * 36.0f * 3.14159f / 180.0f;
            float radius = (i % 2 == 0) ? size : size / 2.5f;
            star << QPointF(x + radius * cos(angle), y + radius * sin(angle));
        }
        painter.drawPolygon(star);
    };
    
    drawStar(150, 100, 20);
    drawStar(width() - 150, 100, 20);
    drawStar(100, 300, 15);
    drawStar(width() - 100, 300, 15);
    drawStar(width() / 2, 420, 18);
    
    // Final Score
    painter.setPen(QColor(255, 255, 255));
    QFont scoreFont;
    scoreFont.setPointSize(32);
    scoreFont.setBold(true);
    painter.setFont(scoreFont);
    
    int centerY = height() / 2 + 50;
    painter.drawText(QRect(0, centerY, width(), 50), Qt::AlignCenter, 
                    QString("Final Score: %1").arg(m_player->score()));
    
    // Highest score
    QFont highScoreFont;
    highScoreFont.setPointSize(24);
    painter.setFont(highScoreFont);
    painter.setPen(QColor(255, 255, 200));
    painter.drawText(QRect(0, centerY + 60, width(), 40), Qt::AlignCenter, 
                    QString("Highest Score: %1").arg(m_highestScore));
    
    // Instructions
    QFont instFont;
    instFont.setPointSize(18);
    painter.setFont(instFont);
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(QRect(0, height() - 80, width(), 40), Qt::AlignCenter, 
                    "Press SPACE to Play Again or ESC to Return to Menu");
}

void GameWidget::keyPressEvent(QKeyEvent* event) {
    // Handle victory screen input
    if (m_showVictoryScreen) {
        if (event->key() == Qt::Key_Space) {
            m_showVictoryScreen = false;
            resetGame();
            return;
        } else if (event->key() == Qt::Key_Escape) {
            m_showVictoryScreen = false;
            // Return to main menu (emit signal or call parent)
            emit returnToMainMenu();
            return;
        }
        return;
    }
    
    // Handle retry screen input
    if (m_showRetryScreen) {
        if (event->key() == Qt::Key_R) {
            m_showRetryScreen = false;
            resetGame();
            return;
        } else if (event->key() == Qt::Key_Escape) {
            m_showRetryScreen = false;
            // Return to main menu (emit signal or call parent)
            emit returnToMainMenu();
            return;
        }
        return;  // Ignore other keys on retry screen
    }
    
    if (!event->isAutoRepeat()) {
        m_pressedKeys.insert(event->key());
    }
    
    // Pause
    if (event->key() == Qt::Key_P || event->key() == Qt::Key_Escape) {
        if (m_gamePaused) {
            resumeGame();
        } else {
            pauseGame();
        }
    }
    
    QWidget::keyPressEvent(event);
}

void GameWidget::keyReleaseEvent(QKeyEvent* event) {
    if (!event->isAutoRepeat()) {
        m_pressedKeys.remove(event->key());
    }
    QWidget::keyReleaseEvent(event);
}

void GameWidget::timerEvent(QTimerEvent* event) {
    QWidget::timerEvent(event);
}
