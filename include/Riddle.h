#ifndef RIDDLE_H
#define RIDDLE_H

#include <QString>
#include <QStringList>
#include <QObject>

class Riddle : public QObject {
    Q_OBJECT

public:
    enum class Difficulty {
        EASY,
        MEDIUM,
        HARD,
        DEADLY
    };

    enum class Type {
        TEXT_ANSWER,
        MULTIPLE_CHOICE,
        CIPHER,
        LOGIC_PUZZLE,
        CODE_CHALLENGE
    };

    explicit Riddle(QObject* parent = nullptr);
    virtual ~Riddle() = default;

    // Getters
    QString getQuestion() const { return m_question; }
    QString getHint() const { return m_hint; }
    QStringList getChoices() const { return m_choices; }
    Difficulty getDifficulty() const { return m_difficulty; }
    Type getType() const { return m_type; }
    int getTimeLimit() const { return m_timeLimit; } // 0 = no limit
    int getDamageOnFailure() const { return m_damageOnFailure; }
    int getRewardPoints() const { return m_rewardPoints; }
    QString getStoryContext() const { return m_storyContext; }

    // Setters
    void setQuestion(const QString& question) { m_question = question; }
    void setHint(const QString& hint) { m_hint = hint; }
    void setChoices(const QStringList& choices) { m_choices = choices; }
    void setAnswer(const QString& answer) { m_answer = answer; }
    void setDifficulty(Difficulty difficulty) { m_difficulty = difficulty; }
    void setType(Type type) { m_type = type; }
    void setTimeLimit(int seconds) { m_timeLimit = seconds; }
    void setDamageOnFailure(int damage) { m_damageOnFailure = damage; }
    void setRewardPoints(int points) { m_rewardPoints = points; }
    void setStoryContext(const QString& context) { m_storyContext = context; }

    // Check answer
    virtual bool checkAnswer(const QString& answer) const;
    bool checkAnswerCaseInsensitive(const QString& answer) const;

signals:
    void riddleSolved();
    void riddleFailed();

protected:
    QString m_question;
    QString m_answer;
    QString m_hint;
    QStringList m_choices;
    Difficulty m_difficulty;
    Type m_type;
    int m_timeLimit;
    int m_damageOnFailure;
    int m_rewardPoints;
    QString m_storyContext;
};

#endif // RIDDLE_H
