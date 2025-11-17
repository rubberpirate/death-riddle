#include "Level.h"
#include "Enemy.h"

Level::Level(int levelNumber, QObject* parent)
    : QObject(parent)
    , m_levelNumber(levelNumber)
    , m_width(30)
    , m_height(20)
    , m_spawnPoint(64, 500)
    , m_complete(false)
    , m_totalCoins(0)
    , m_coinsCollected(0)
{
    // Initialize empty grid
    m_tiles.resize(m_height);
    for (int y = 0; y < m_height; ++y) {
        m_tiles[y].resize(m_width);
        for (int x = 0; x < m_width; ++x) {
            m_tiles[y][x].gridPos = QPoint(x, y);
            m_tiles[y][x].boundingBox = QRectF(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        }
    }
    
    loadLevel(levelNumber);
}

void Level::addEnemy(Enemy* enemy) {
    m_enemies.append(enemy);
}

void Level::loadLevel(int levelNumber) {
    switch (levelNumber) {
        case 1: createLevel1(); break;
        case 2: createLevel2(); break;
        case 3: createLevel3(); break;
        case 4: createLevel4(); break;
        case 5: createLevel5(); break;
        case 6: createLevel6(); break;
        default: createLevel1(); break;
    }
}

Tile* Level::getTileAt(int x, int y) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return nullptr;
    }
    return &m_tiles[y][x];
}

const Tile* Level::getTileAt(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return nullptr;
    }
    return &m_tiles[y][x];
}

Tile* Level::getTileAtPixel(float x, float y) {
    int gridX = static_cast<int>(x / TILE_SIZE);
    int gridY = static_cast<int>(y / TILE_SIZE);
    return getTileAt(gridX, gridY);
}

QVector<Tile*> Level::getTilesInArea(const QRectF& area) {
    QVector<Tile*> result;
    
    int startX = static_cast<int>(area.left() / TILE_SIZE);
    int endX = static_cast<int>(area.right() / TILE_SIZE);
    int startY = static_cast<int>(area.top() / TILE_SIZE);
    int endY = static_cast<int>(area.bottom() / TILE_SIZE);
    
    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            Tile* tile = getTileAt(x, y);
            if (tile) {
                result.append(tile);
            }
        }
    }
    
    return result;
}

QVector<Tile*> Level::getSolidTiles() {
    QVector<Tile*> result;
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (m_tiles[y][x].type == TileType::SOLID || 
                m_tiles[y][x].type == TileType::MOVING_PLATFORM) {
                result.append(&m_tiles[y][x]);
            }
        }
    }
    return result;
}

bool Level::isSolid(int gridX, int gridY) const {
    const Tile* tile = getTileAt(gridX, gridY);
    if (!tile) return false;
    return tile->type == TileType::SOLID || tile->type == TileType::MOVING_PLATFORM;
}

bool Level::checkCollision(const QRectF& box, TileType& hitType) {
    QVector<Tile*> tiles = getTilesInArea(box);
    
    for (Tile* tile : tiles) {
        if (tile->type != TileType::EMPTY && tile->boundingBox.intersects(box)) {
            hitType = tile->type;
            return true;
        }
    }
    
    return false;
}

void Level::collectCoin(Tile* coin) {
    if (coin && coin->type == TileType::COIN && !coin->collected) {
        coin->collected = true;
        m_coinsCollected++;
        emit coinCollected(m_totalCoins - m_coinsCollected);
    }
}

void Level::activateCheckpoint(Tile* checkpoint) {
    if (checkpoint && checkpoint->type == TileType::CHECKPOINT && !checkpoint->activated) {
        checkpoint->activated = true;
        emit checkpointActivated(QPointF(checkpoint->gridPos.x() * TILE_SIZE, 
                                         checkpoint->gridPos.y() * TILE_SIZE));
    }
}

int Level::coinsRemaining() const {
    return m_totalCoins - m_coinsCollected;
}

void Level::setTile(int x, int y, TileType type, int riddleId) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;
    
    m_tiles[y][x].type = type;
    m_tiles[y][x].riddleId = riddleId;
    
    if (type == TileType::COIN) {
        m_totalCoins++;
    }
}

void Level::createLevel1() {
    m_name = "Level 1: The Awakening";
    m_description = "Learn the basics. Move with arrow keys, collect coins, reach the goal!";
    m_spawnPoint = QPointF(64, 500);
    
    // Ground
    for (int x = 0; x < m_width; ++x) {
        setTile(x, 18, TileType::SOLID);
        setTile(x, 19, TileType::SOLID);
    }
    
    // Starting platform
    for (int x = 0; x < 5; ++x) {
        setTile(x, 17, TileType::SOLID);
    }
    
    // First gap with coins
    setTile(5, 16, TileType::COIN);
    setTile(6, 15, TileType::COIN);
    setTile(7, 16, TileType::COIN);
    
    // Landing platform
    for (int x = 8; x < 13; ++x) {
        setTile(x, 17, TileType::SOLID);
    }
    
    // Stairs up
    for (int i = 0; i < 5; ++i) {
        for (int x = 13 + i; x < 15 + i; ++x) {
            setTile(x, 17 - i, TileType::SOLID);
        }
    }
    
    // Upper platform with enemy
    for (int x = 18; x < 25; ++x) {
        setTile(x, 12, TileType::SOLID);
    }
    // Add enemy (pink monster) with riddle 0
    Enemy* enemy1 = new Enemy(Enemy::Type::PINK_MONSTER, QPointF(21 * TILE_SIZE, 11 * TILE_SIZE), 0, this);
    addEnemy(enemy1);
    setTile(23, 11, TileType::COIN);
    
    // Path to goal
    for (int x = 25; x < m_width; ++x) {
        setTile(x, 15, TileType::SOLID);
    }
    
    // Goal
    setTile(28, 14, TileType::GOAL);
}

void Level::createLevel2() {
    m_name = "Level 2: The Cipher Challenge";
    m_description = "Navigate platforms and solve the ROT13 riddle!";
    m_spawnPoint = QPointF(64, 500);
    
    // Ground
    for (int x = 0; x < m_width; ++x) {
        setTile(x, 19, TileType::SOLID);
    }
    
    // Starting area
    for (int x = 0; x < 6; ++x) {
        setTile(x, 18, TileType::SOLID);
    }
    
    // Floating platforms with gaps
    for (int x = 7; x < 10; ++x) {
        setTile(x, 16, TileType::SOLID);
    }
    setTile(8, 15, TileType::COIN);
    
    for (int x = 12; x < 15; ++x) {
        setTile(x, 14, TileType::SOLID);
    }
    
    // Spike pit
    for (int x = 16; x < 20; ++x) {
        setTile(x, 18, TileType::SPIKE);
    }
    
    // Platform over spikes
    for (int x = 16; x < 20; ++x) {
        setTile(x, 15, TileType::SOLID);
    }
    setTile(17, 14, TileType::COIN);
    // Add enemy (owlet monster) with riddle 1 - Cipher riddle
    Enemy* enemy2 = new Enemy(Enemy::Type::OWLET_MONSTER, QPointF(18 * TILE_SIZE, 14 * TILE_SIZE), 1, this);
    addEnemy(enemy2);
    
    // Path to goal
    for (int x = 20; x < m_width; ++x) {
        setTile(x, 16, TileType::SOLID);
    }
    setTile(28, 15, TileType::GOAL);
}

void Level::createLevel3() {
    m_name = "Level 3: Logic Leap";
    m_description = "Test your jumping skills and logical thinking!";
    m_spawnPoint = QPointF(64, 500);
    
    // Ground with gaps
    for (int x = 0; x < 8; ++x) {
        setTile(x, 18, TileType::SOLID);
    }
    
    // Series of platforms
    for (int x = 10; x < 13; ++x) {
        setTile(x, 16, TileType::SOLID);
    }
    
    for (int x = 15; x < 18; ++x) {
        setTile(x, 14, TileType::SOLID);
    }
    
    for (int x = 20; x < 23; ++x) {
        setTile(x, 12, TileType::SOLID);
    }
    // Add enemy (pink monster) with riddle 2 - Logic riddle
    Enemy* enemy3 = new Enemy(Enemy::Type::PINK_MONSTER, QPointF(21 * TILE_SIZE, 11 * TILE_SIZE), 2, this);
    addEnemy(enemy3);
    
    // Checkpoint
    setTile(21, 10, TileType::CHECKPOINT);
    
    // Goal platform
    for (int x = 26; x < m_width; ++x) {
        setTile(x, 10, TileType::SOLID);
    }
    setTile(28, 9, TileType::GOAL);
}

void Level::createLevel4() {
    m_name = "Level 4: Binary Bridge";
    m_description = "Cross the binary bridge and decode the message!";
    m_spawnPoint = QPointF(64, 500);
    
    // Starting platform
    for (int x = 0; x < 6; ++x) {
        for (int y = 17; y < 20; ++y) {
            setTile(x, y, TileType::SOLID);
        }
    }
    
    // Spike floor
    for (int x = 6; x < 24; ++x) {
        setTile(x, 19, TileType::SPIKE);
    }
    
    // Floating platforms (binary pattern)
    setTile(7, 16, TileType::SOLID);   // 0
    setTile(9, 16, TileType::SOLID);   // 1
    setTile(11, 16, TileType::SOLID);  // 0
    setTile(13, 16, TileType::SOLID);  // 0
    
    setTile(15, 14, TileType::SOLID);  // 0
    setTile(17, 14, TileType::SOLID);  // 1
    setTile(19, 14, TileType::SOLID);  // 0
    setTile(21, 14, TileType::SOLID);  // 1
    
    // Riddle platform
    for (int x = 23; x < 27; ++x) {
        setTile(x, 13, TileType::SOLID);
    }
    // Enemy with binary riddle
    Enemy* enemy4 = new Enemy(Enemy::Type::OWLET_MONSTER, QPointF(25 * TILE_SIZE, 12 * TILE_SIZE), 3, this);
    addEnemy(enemy4);
    
    // Goal
    setTile(28, 15, TileType::SOLID);
    setTile(28, 14, TileType::GOAL);
}

void Level::createLevel5() {
    m_name = "Level 5: The Final Test";
    m_description = "Solve the ultimate riddle and escape from the Game Master!";
    m_spawnPoint = QPointF(64, 500);
    
    // Complex level with all elements
    for (int x = 0; x < m_width; ++x) {
        setTile(x, 19, TileType::SOLID);
    }
    
    // Multiple paths
    for (int x = 0; x < 8; ++x) {
        setTile(x, 17, TileType::SOLID);
    }
    
    // Upper path (harder, more coins)
    for (int x = 9; x < 14; ++x) {
        setTile(x, 12, TileType::SOLID);
    }
    setTile(10, 11, TileType::COIN);
    setTile(11, 11, TileType::COIN);
    setTile(12, 11, TileType::COIN);
    
    // Lower path (easier, spikes)
    for (int x = 9; x < 14; ++x) {
        setTile(x, 16, TileType::SOLID);
        if (x % 2 == 0) {
            setTile(x, 15, TileType::SPIKE);
        }
    }
    
    // Central platform with final riddle
    for (int x = 16; x < 22; ++x) {
        for (int y = 13; y < 15; ++y) {
            setTile(x, y, TileType::SOLID);
        }
    }
    // Final enemy with riddle
    Enemy* enemy5 = new Enemy(Enemy::Type::PINK_MONSTER, QPointF(19 * TILE_SIZE, 12 * TILE_SIZE), 4, this);
    addEnemy(enemy5);
    
    // Goal
    for (int x = 24; x < m_width; ++x) {
        setTile(x, 15, TileType::SOLID);
    }
    setTile(28, 14, TileType::GOAL);
}

void Level::createLevel6() {
    m_name = "Level 6: The Labyrinth";
    m_description = "Navigate the maze and find the key to escape!";
    
    // Simple, clean maze design with open passages
    int offsetX = 1;
    int offsetY = 1;
    int mazeWidth = 28;
    int mazeHeight = 18;
    
    // Set spawn point inside the maze at top-left
    m_spawnPoint = QPointF((offsetX + 1) * TILE_SIZE + 8, (offsetY + 1) * TILE_SIZE + 8);
    
    // Outer boundary
    for (int x = offsetX; x < offsetX + mazeWidth; ++x) {
        setTile(x, offsetY, TileType::SOLID);  // Top wall
        setTile(x, offsetY + mazeHeight - 1, TileType::SOLID);  // Bottom wall
    }
    for (int y = offsetY; y < offsetY + mazeHeight; ++y) {
        setTile(offsetX, y, TileType::SOLID);  // Left wall
        setTile(offsetX + mazeWidth - 1, y, TileType::SOLID);  // Right wall
    }
    
    // Create maze with short wall segments and open passages
    // Think of it as "L" and "T" shaped wall pieces with gaps
    
    // Top-left area walls
    for (int x = offsetX + 2; x < offsetX + 5; ++x) {
        setTile(x, offsetY + 3, TileType::SOLID);
    }
    for (int y = offsetY + 3; y < offsetY + 6; ++y) {
        setTile(offsetX + 5, y, TileType::SOLID);
    }
    
    // Top-center area walls
    for (int x = offsetX + 8; x < offsetX + 11; ++x) {
        setTile(x, offsetY + 2, TileType::SOLID);
    }
    for (int y = offsetY + 4; y < offsetY + 7; ++y) {
        setTile(offsetX + 10, y, TileType::SOLID);
    }
    
    // Top-right area walls  
    for (int x = offsetX + 14; x < offsetX + 17; ++x) {
        setTile(x, offsetY + 3, TileType::SOLID);
    }
    for (int y = offsetY + 3; y < offsetY + 6; ++y) {
        setTile(offsetX + 17, y, TileType::SOLID);
    }
    
    for (int x = offsetX + 21; x < offsetX + 24; ++x) {
        setTile(x, offsetY + 2, TileType::SOLID);
    }
    for (int y = offsetY + 4; y < offsetY + 7; ++y) {
        setTile(offsetX + 23, y, TileType::SOLID);
    }
    
    // Middle-left area walls
    for (int x = offsetX + 2; x < offsetX + 5; ++x) {
        setTile(x, offsetY + 9, TileType::SOLID);
    }
    for (int y = offsetY + 9; y < offsetY + 12; ++y) {
        setTile(offsetX + 5, y, TileType::SOLID);
    }
    
    // Center area walls (around key)
    for (int x = offsetX + 8; x < offsetX + 11; ++x) {
        setTile(x, offsetY + 8, TileType::SOLID);
    }
    for (int y = offsetY + 10; y < offsetY + 13; ++y) {
        setTile(offsetX + 10, y, TileType::SOLID);
    }
    
    for (int x = offsetX + 17; x < offsetX + 20; ++x) {
        setTile(x, offsetY + 8, TileType::SOLID);
    }
    for (int y = offsetY + 10; y < offsetY + 13; ++y) {
        setTile(offsetX + 17, y, TileType::SOLID);
    }
    
    // Middle-right area walls
    for (int x = offsetX + 23; x < offsetX + 26; ++x) {
        setTile(x, offsetY + 9, TileType::SOLID);
    }
    for (int y = offsetY + 9; y < offsetY + 12; ++y) {
        setTile(offsetX + 23, y, TileType::SOLID);
    }
    
    // Bottom-left area walls
    for (int x = offsetX + 2; x < offsetX + 5; ++x) {
        setTile(x, offsetY + 15, TileType::SOLID);
    }
    for (int y = offsetY + 12; y < offsetY + 15; ++y) {
        setTile(offsetX + 5, y, TileType::SOLID);
    }
    
    // Bottom-center area walls
    for (int x = offsetX + 8; x < offsetX + 11; ++x) {
        setTile(x, offsetY + 14, TileType::SOLID);
    }
    for (int y = offsetY + 14; y < offsetY + 17; ++y) {
        setTile(offsetX + 10, y, TileType::SOLID);
    }
    
    for (int x = offsetX + 17; x < offsetX + 20; ++x) {
        setTile(x, offsetY + 14, TileType::SOLID);
    }
    for (int y = offsetY + 14; y < offsetY + 17; ++y) {
        setTile(offsetX + 17, y, TileType::SOLID);
    }
    
    // Place the KEY in the center with open access from all sides
    setTile(offsetX + 14, offsetY + 9, TileType::KEY);
    
    // Place coins as path markers
    setTile(offsetX + 3, offsetY + 2, TileType::COIN);
    setTile(offsetX + 12, offsetY + 5, TileType::COIN);
    setTile(offsetX + 7, offsetY + 10, TileType::COIN);
    setTile(offsetX + 20, offsetY + 10, TileType::COIN);
    setTile(offsetX + 14, offsetY + 15, TileType::COIN);
    
    // Place the GOAL in bottom-right corner
    setTile(offsetX + 26, offsetY + 16, TileType::GOAL);
}
