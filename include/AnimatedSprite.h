#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <QPixmap>
#include <QString>
#include <QRect>
#include <QVector>

class AnimatedSprite {
public:
    AnimatedSprite();
    ~AnimatedSprite();
    
    // Load a sprite sheet and configure frame dimensions
    bool loadSpriteSheet(const QString& filePath, int frameWidth, int frameHeight, int frameCount);
    
    // Update animation (call each frame)
    void update(float deltaTime);
    
    // Get current frame to draw
    QRect getCurrentFrame() const;
    
    // Get the sprite sheet pixmap
    const QPixmap& getPixmap() const { return m_spriteSheet; }
    
    // Control
    void play();
    void pause();
    void reset();
    void setLoop(bool loop) { m_loop = loop; }
    void setFrameRate(float fps) { m_frameTime = 1.0f / fps; }
    
    // Info
    bool isPlaying() const { return m_playing; }
    bool isFinished() const { return m_finished; }
    int currentFrameIndex() const { return m_currentFrame; }
    int frameCount() const { return m_frameCount; }
    
    int frameWidth() const { return m_frameWidth; }
    int frameHeight() const { return m_frameHeight; }

private:
    QPixmap m_spriteSheet;
    int m_frameWidth;
    int m_frameHeight;
    int m_frameCount;
    int m_currentFrame;
    float m_elapsedTime;
    float m_frameTime;
    bool m_playing;
    bool m_loop;
    bool m_finished;
};

#endif // ANIMATEDSPRITE_H
