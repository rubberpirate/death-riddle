#ifndef LEVEL_H
#define LEVEL_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QRectF>
#include <QString>

class Enemy;

enum class TileType {
    EMPTY,
    SOLID,           // Regular platform
    SPIKE,           // Damages player
    COIN,            // Collectible
    CHECKPOINT,      // Save point
    RIDDLE_TRIGGER,  // Triggers riddle challenge
    GOAL,            // Level end
    MOVING_PLATFORM,
    BREAKABLE,
    KEY              // Key to unlock goal
};

struct Tile {
    TileType type;
    QPoint gridPos;
    QRectF boundingBox;
    bool collected;  // For coins
    bool activated;  // For checkpoints/triggers
    int riddleId;    // Which riddle this triggers
    
    Tile() : type(TileType::EMPTY), collected(false), activated(false), riddleId(-1) {}
};

class Level : public QObject {
    Q_OBJECT

public:
    explicit Level(int levelNumber, QObject* parent = nullptr);
    
    // Level info
    int levelNumber() const { return m_levelNumber; }
    QString name() const { return m_name; }
    QString description() const { return m_description; }
    QPointF spawnPoint() const { return m_spawnPoint; }
    
    // Tile system
    static constexpr int TILE_SIZE = 32;
    int width() const { return m_width; }
    int height() const { return m_height; }
    
    Tile* getTileAt(int x, int y);
    const Tile* getTileAt(int x, int y) const;
    Tile* getTileAtPixel(float x, float y);
    
    QVector<Tile*> getTilesInArea(const QRectF& area);
    QVector<Tile*> getSolidTiles();
    
    // Collision detection
    bool isSolid(int gridX, int gridY) const;
    bool checkCollision(const QRectF& box, TileType& hitType);
    
    // Level state
    void collectCoin(Tile* coin);
    void activateCheckpoint(Tile* checkpoint);
    int coinsRemaining() const;
    bool isComplete() const { return m_complete; }
    void setComplete(bool complete) { m_complete = complete; }
    
    // Level creation
    void setTile(int x, int y, TileType type, int riddleId = -1);
    void loadLevel(int levelNumber);
    
    // Enemy management
    void addEnemy(Enemy* enemy);
    QVector<Enemy*>& enemies() { return m_enemies; }
    const QVector<Enemy*>& enemies() const { return m_enemies; }

signals:
    void coinCollected(int remaining);
    void checkpointActivated(QPointF position);
    void riddleTriggered(int riddleId, Enemy* enemy);
    void levelComplete();

private:
    void createLevel1();  // Tutorial level
    void createLevel2();  // First riddle challenge
    void createLevel3();  // Moving platforms
    void createLevel4();  // Spike gauntlet
    void createLevel5();  // Final challenge
    void createLevel6();  // Top-down maze with key
    
    int m_levelNumber;
    QString m_name;
    QString m_description;
    int m_width;
    int m_height;
    QVector<QVector<Tile>> m_tiles;
    QVector<Enemy*> m_enemies;
    QPointF m_spawnPoint;
    bool m_complete;
    int m_totalCoins;
    int m_coinsCollected;
};

#endif // LEVEL_H
