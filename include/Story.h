#ifndef STORY_H
#define STORY_H

#include "Riddle.h"
#include <QObject>
#include <QVector>
#include <QMap>

class Story : public QObject {
    Q_OBJECT

public:
    enum class Ending {
        NONE,
        TRUE_FREEDOM,      // Perfect escape
        THE_DEAL,          // Become apprentice
        PARTIAL_ESCAPE,    // Escape with cost
        THE_LOOP,          // Trapped forever
        DEATHS_EMBRACE     // Game over
    };

    struct StoryNode {
        int id;
        QString narrative;
        QString gameMasterDialogue;
        QVector<QString> choices;
        QVector<int> nextNodeIds;
        Riddle* riddle;        // nullptr if no riddle
        int healthChange;      // Can be positive or negative
        bool isEnding;
        Ending endingType;
    };

    explicit Story(QObject* parent = nullptr);
    virtual ~Story();

    // Story progression
    void startStory();
    const StoryNode* getCurrentNode() const;
    void makeChoice(int choiceIndex);
    bool hasRiddle() const;
    Riddle* getCurrentRiddle() const;
    void solveRiddle(bool success);

    // Ending conditions
    bool isGameEnded() const { return m_gameEnded; }
    Ending getEnding() const { return m_currentEnding; }
    QString getEndingDescription(Ending ending) const;

    // Progress tracking
    int getCurrentNodeId() const { return m_currentNodeId; }
    QVector<int> getVisitedNodes() const { return m_visitedNodes; }

signals:
    void storyProgressed(int newNodeId);
    void endingReached(Ending ending);
    void narrativeChanged(QString narrative);
    void choicesAvailable(QVector<QString> choices);

private:
    void initializeStory();
    void createRiddles();
    void goToNode(int nodeId);

    QMap<int, StoryNode*> m_storyNodes;
    QVector<Riddle*> m_riddles;
    int m_currentNodeId;
    QVector<int> m_visitedNodes;
    bool m_gameEnded;
    Ending m_currentEnding;
};

#endif // STORY_H
