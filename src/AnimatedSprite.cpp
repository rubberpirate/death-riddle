#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite()
    : m_frameWidth(0)
    , m_frameHeight(0)
    , m_frameCount(0)
    , m_currentFrame(0)
    , m_elapsedTime(0.0f)
    , m_frameTime(1.0f / 10.0f)  // Default 10 FPS
    , m_playing(false)
    , m_loop(true)
    , m_finished(false)
{
}

AnimatedSprite::~AnimatedSprite() {
}

bool AnimatedSprite::loadSpriteSheet(const QString& filePath, int frameWidth, int frameHeight, int frameCount) {
    if (!m_spriteSheet.load(filePath)) {
        return false;
    }
    
    m_frameWidth = frameWidth;
    m_frameHeight = frameHeight;
    m_frameCount = frameCount;
    m_currentFrame = 0;
    m_elapsedTime = 0.0f;
    m_finished = false;
    
    return true;
}

void AnimatedSprite::update(float deltaTime) {
    if (!m_playing || m_finished) {
        return;
    }
    
    m_elapsedTime += deltaTime;
    
    if (m_elapsedTime >= m_frameTime) {
        m_elapsedTime -= m_frameTime;
        m_currentFrame++;
        
        if (m_currentFrame >= m_frameCount) {
            if (m_loop) {
                m_currentFrame = 0;
            } else {
                m_currentFrame = m_frameCount - 1;
                m_finished = true;
                m_playing = false;
            }
        }
    }
}

QRect AnimatedSprite::getCurrentFrame() const {
    int x = m_currentFrame * m_frameWidth;
    int y = 0;
    return QRect(x, y, m_frameWidth, m_frameHeight);
}

void AnimatedSprite::play() {
    m_playing = true;
    m_finished = false;
}

void AnimatedSprite::pause() {
    m_playing = false;
}

void AnimatedSprite::reset() {
    m_currentFrame = 0;
    m_elapsedTime = 0.0f;
    m_finished = false;
}

