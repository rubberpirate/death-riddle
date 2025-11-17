#include "GameState.h"

GameState::GameState(QObject* parent)
    : QObject(parent)
    , m_player(new Player(this))
    , m_gameMaster(new GameMaster(this))
    , m_story(new Story(this))
    , m_state(State::MENU)
    , m_paused(false)
    , m_hintsRemaining(3)
    , m_currentAttempts(0)
{
    connectSignals();
}

void GameState::connectSignals() {
    // Player signals
    connect(m_player, &Player::playerDied, this, &GameState::onPlayerDied);
    connect(m_player, &Player::healthChanged, this, &GameState::onHealthChanged);

    // Story signals
    connect(m_story, &Story::endingReached, this, &GameState::onStoryEnding);
    connect(m_story, &Story::narrativeChanged, this, &GameState::narrativeUpdate);

    // GameMaster signals
    connect(m_gameMaster, &GameMaster::dialogueChanged, this, &GameState::gameMasterSpeaks);
}

void GameState::startNewGame() {
    m_player->reset();
    m_hintsRemaining = 3;
    m_currentAttempts = 0;
    m_paused = false;

    m_story->startStory();
    setState(State::DIALOGUE);

    // Set initial Game Master dialogue
    const Story::StoryNode* node = m_story->getCurrentNode();
    if (node) {
        m_gameMaster->setDialogue(node->gameMasterDialogue);
    }

    emit gameStarted();
}

void GameState::continueGame() {
    setState(State::PLAYING);
    m_paused = false;
}

void GameState::pauseGame() {
    m_paused = true;
}

void GameState::resumeGame() {
    m_paused = false;
}

void GameState::endGame() {
    setState(State::MENU);
}

void GameState::setState(State state) {
    if (m_state != state) {
        m_state = state;
        emit stateChanged(state);
    }
}

void GameState::submitAnswer(const QString& answer) {
    if (m_state != State::RIDDLE_ACTIVE) {
        return;
    }

    Riddle* riddle = m_story->getCurrentRiddle();
    if (!riddle) {
        return;
    }

    bool correct = riddle->checkAnswerCaseInsensitive(answer);

    if (correct) {
        // Correct answer
        m_player->addScore(riddle->getRewardPoints());
        m_player->incrementRiddlesSolved();
        m_player->setState(Player::State::CELEBRATING);

        QString feedback = m_gameMaster->reactToCorrectAnswer();
        emit riddleResult(true, feedback);

        // Progress story
        m_story->solveRiddle(true);
        setState(State::DIALOGUE);

    } else {
        // Wrong answer
        m_currentAttempts++;
        m_player->takeDamage(riddle->getDamageOnFailure());
        m_player->incrementFailedAttempts();

        QString feedback = m_gameMaster->reactToWrongAnswer();
        emit riddleResult(false, feedback);

        // Check if player died
        if (!m_player->isAlive()) {
            onPlayerDied();
        } else {
            // Allow retry
            m_player->setState(Player::State::THINKING);
        }
    }
}

void GameState::requestHint() {
    if (m_hintsRemaining <= 0) {
        emit hintProvided("No hints remaining!");
        return;
    }

    Riddle* riddle = m_story->getCurrentRiddle();
    if (!riddle) {
        return;
    }

    m_hintsRemaining--;
    m_player->takeDamage(5); // Small penalty for hint

    QString hint = riddle->getHint();
    emit hintProvided(hint);

    m_gameMaster->setDialogue("A hint... but it costs you.");
    m_gameMaster->setMood(GameMaster::Mood::AMUSED);
}

void GameState::processChoice(int choiceIndex) {
    if (m_state != State::DIALOGUE && m_state != State::PLAYING) {
        return;
    }

    const Story::StoryNode* node = m_story->getCurrentNode();
    if (!node) {
        return;
    }

    // Apply health changes
    if (node->healthChange != 0) {
        if (node->healthChange > 0) {
            m_player->heal(node->healthChange);
        } else {
            m_player->takeDamage(-node->healthChange);
        }
    }

    // Set Game Master dialogue
    m_gameMaster->setDialogue(node->gameMasterDialogue);

    // Make the choice
    m_story->makeChoice(choiceIndex);

    // Check if next node has a riddle
    if (m_story->hasRiddle()) {
        setState(State::RIDDLE_ACTIVE);
        Riddle* riddle = m_story->getCurrentRiddle();
        if (riddle) {
            m_gameMaster->setDialogue(riddle->getStoryContext());
        }
    } else {
        setState(State::DIALOGUE);
    }

    checkGameOver();
    checkEnding();
}

void GameState::checkGameOver() {
    if (!m_player->isAlive() && m_state != State::GAME_OVER) {
        setState(State::GAME_OVER);
        emit gameOver(Story::Ending::DEATHS_EMBRACE);
    }
}

void GameState::checkEnding() {
    if (m_story->isGameEnded()) {
        setState(State::ENDING);
        emit gameOver(m_story->getEnding());
    }
}

void GameState::onPlayerDied() {
    setState(State::GAME_OVER);
    m_gameMaster->setDialogue("Your life force has faded. You are mine now...");
    m_gameMaster->setMood(GameMaster::Mood::SATISFIED);
    emit gameOver(Story::Ending::DEATHS_EMBRACE);
}

void GameState::onStoryEnding(Story::Ending ending) {
    setState(State::ENDING);
    
    switch (ending) {
        case Story::Ending::TRUE_FREEDOM:
            m_gameMaster->setMood(GameMaster::Mood::IMPRESSED);
            break;
        case Story::Ending::THE_DEAL:
            m_gameMaster->setMood(GameMaster::Mood::SATISFIED);
            break;
        default:
            m_gameMaster->setMood(GameMaster::Mood::NEUTRAL);
            break;
    }
}

void GameState::onHealthChanged(int health, int maxHealth) {
    float healthPercentage = static_cast<float>(health) / maxHealth;
    
    if (healthPercentage <= 0.25f && health > 0) {
        m_gameMaster->setDialogue(m_gameMaster->reactToLowHealth());
    } else if (healthPercentage >= 0.9f && m_player->getRiddlesSolved() > 2) {
        m_gameMaster->setDialogue(m_gameMaster->reactToHighScore());
    }
}
