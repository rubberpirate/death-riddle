#include "GameMaster.h"
#include <QRandomGenerator>

GameMaster::GameMaster(QObject* parent)
    : QObject(parent)
    , m_mood(Mood::NEUTRAL)
    , m_isAnimating(false)
{
    m_currentDialogue = "Welcome to my realm...";
}

void GameMaster::setDialogue(const QString& dialogue) {
    m_currentDialogue = dialogue;
    emit dialogueChanged(dialogue);
}

void GameMaster::setMood(Mood mood) {
    if (m_mood != mood) {
        m_mood = mood;
        emit moodChanged(mood);
    }
}

QString GameMaster::reactToCorrectAnswer() {
    QStringList responses = {
        "Impressive... but don't get cocky.",
        "Correct. You might actually survive this.",
        "Well done. Few make it this far.",
        "You're smarter than you look.",
        "Hmm... perhaps you're worthy after all."
    };
    
    setMood(Mood::IMPRESSED);
    return responses.at(QRandomGenerator::global()->bounded(responses.size()));
}

QString GameMaster::reactToWrongAnswer() {
    QStringList responses = {
        "Wrong. Feel the consequences.",
        "Pathetic. Did you even try?",
        "No... that's not even close.",
        "Your ignorance is painful to watch.",
        "I expected better from you."
    };
    
    setMood(Mood::ANGRY);
    return responses.at(QRandomGenerator::global()->bounded(responses.size()));
}

QString GameMaster::reactToLowHealth() {
    QStringList responses = {
        "You're fading... how amusing.",
        "Can you feel death approaching?",
        "One more mistake, and you're mine forever.",
        "Your life force wanes... delightful.",
        "Weakness. I can smell it."
    };
    
    setMood(Mood::MENACING);
    return responses.at(QRandomGenerator::global()->bounded(responses.size()));
}

QString GameMaster::reactToHighScore() {
    QStringList responses = {
        "You've impressed me. That's... rare.",
        "Such skill. Perhaps you deserve a choice.",
        "Remarkable. You might be worthy of power.",
        "Few have shown such prowess.",
        "You're becoming interesting..."
    };
    
    setMood(Mood::SATISFIED);
    return responses.at(QRandomGenerator::global()->bounded(responses.size()));
}

void GameMaster::startAnimation() {
    if (!m_isAnimating) {
        m_isAnimating = true;
        emit animationStateChanged(true);
    }
}

void GameMaster::stopAnimation() {
    if (m_isAnimating) {
        m_isAnimating = false;
        emit animationStateChanged(false);
    }
}
