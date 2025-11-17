#include "Riddle.h"

Riddle::Riddle(QObject* parent)
    : QObject(parent)
    , m_difficulty(Difficulty::MEDIUM)
    , m_type(Type::TEXT_ANSWER)
    , m_timeLimit(0)
    , m_damageOnFailure(15)
    , m_rewardPoints(100)
{
}

bool Riddle::checkAnswer(const QString& answer) const {
    return answer.trimmed() == m_answer.trimmed();
}

bool Riddle::checkAnswerCaseInsensitive(const QString& answer) const {
    return answer.trimmed().toLower() == m_answer.trimmed().toLower();
}
