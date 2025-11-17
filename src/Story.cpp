#include "Story.h"

Story::Story(QObject* parent)
    : QObject(parent)
    , m_currentNodeId(0)
    , m_gameEnded(false)
    , m_currentEnding(Ending::NONE)
{
    createRiddles();
    initializeStory();
}

Story::~Story() {
    qDeleteAll(m_riddles);
    qDeleteAll(m_storyNodes);
}

void Story::createRiddles() {
    // Riddle 1: Easy warmup
    Riddle* riddle1 = new Riddle(this);
    riddle1->setQuestion("What has keys but no locks, space but no room, and you can enter but can't go inside?");
    riddle1->setAnswer("keyboard");
    riddle1->setHint("It's something you're using right now...");
    riddle1->setDifficulty(Riddle::Difficulty::EASY);
    riddle1->setDamageOnFailure(10);
    riddle1->setRewardPoints(50);
    riddle1->setStoryContext("The Game Master tests your wit with a simple riddle...");
    m_riddles.append(riddle1);

    // Riddle 2: Cipher challenge
    Riddle* riddle2 = new Riddle(this);
    riddle2->setQuestion("Decode this message: 'GUVF VF ABG SERR'\nCipher: ROT13");
    riddle2->setAnswer("THIS IS NOT FREE");
    riddle2->setHint("Each letter is shifted by 13 positions in the alphabet");
    riddle2->setDifficulty(Riddle::Difficulty::MEDIUM);
    riddle2->setType(Riddle::Type::CIPHER);
    riddle2->setDamageOnFailure(15);
    riddle2->setRewardPoints(100);
    riddle2->setStoryContext("A cryptic message appears before you...");
    m_riddles.append(riddle2);

    // Riddle 3: Logic puzzle
    Riddle* riddle3 = new Riddle(this);
    riddle3->setQuestion("Three gates: Gold, Silver, Bronze.\nGold says: 'Freedom lies behind Silver'\nSilver says: 'Freedom is not here'\nBronze says: 'Gold speaks truth'\nOnly ONE speaks truth. Which gate leads to freedom?");
    riddle3->setAnswer("gold");
    riddle3->setHint("If Bronze is true, then Gold is true - but only one can be true...");
    riddle3->setDifficulty(Riddle::Difficulty::HARD);
    riddle3->setType(Riddle::Type::LOGIC_PUZZLE);
    riddle3->setDamageOnFailure(20);
    riddle3->setRewardPoints(150);
    riddle3->setStoryContext("The Game Master presents you with three gates...");
    m_riddles.append(riddle3);

    // Riddle 4: Binary puzzle
    Riddle* riddle4 = new Riddle(this);
    riddle4->setQuestion("Convert this binary to text: 01000110 01010010 01000101 01000101");
    riddle4->setAnswer("FREE");
    riddle4->setHint("8 bits = 1 ASCII character");
    riddle4->setDifficulty(Riddle::Difficulty::MEDIUM);
    riddle4->setType(Riddle::Type::CODE_CHALLENGE);
    riddle4->setDamageOnFailure(15);
    riddle4->setRewardPoints(120);
    riddle4->setStoryContext("Numbers dance before your eyes...");
    m_riddles.append(riddle4);

    // Riddle 5: Final riddle
    Riddle* riddle5 = new Riddle(this);
    riddle5->setQuestion("I am the beginning of eternity, the end of time and space.\nI am the beginning of every end, the end of every place. What am I?");
    riddle5->setAnswer("e");
    riddle5->setHint("Look at the first and last letters...");
    riddle5->setDifficulty(Riddle::Difficulty::DEADLY);
    riddle5->setDamageOnFailure(25);
    riddle5->setRewardPoints(200);
    riddle5->setStoryContext("The final test. Your destiny hangs in the balance...");
    m_riddles.append(riddle5);
}

void Story::initializeStory() {
    // Node 0: Introduction
    StoryNode* node0 = new StoryNode{
        0,
        "You awaken in an endless void, surrounded by swirling shadows.\n\n"
        "A figure materializes before you - tall, cloaked in darkness, with eyes that pierce through your soul.",
        "Welcome, mortal. I am the Game Master, and you are now mine.\n\n"
        "Your previous life? A distant memory. Your future? That depends on your wit.\n\n"
        "Solve my riddles, and perhaps you'll earn your freedom... or something else entirely.",
        {"I'll play your game", "Let me go!", "Who are you really?"},
        {1, 1, 2},
        nullptr,
        0,
        false,
        Ending::NONE
    };
    m_storyNodes[0] = node0;

    // Node 1: First riddle intro
    StoryNode* node1 = new StoryNode{
        1,
        "The Game Master's lips curl into a sinister smile.",
        "Good. Let's see if you're as clever as you think.\n\n"
        "Here's your first riddle. Fail, and you'll feel pain. Succeed, and we continue.",
        {},
        {3},
        m_riddles[0],
        0,
        false,
        Ending::NONE
    };
    m_storyNodes[1] = node1;

    // Node 2: Defiant response
    StoryNode* node2 = new StoryNode{
        2,
        "The Game Master laughs, a sound that echoes through infinity.",
        "You dare question me? Such spirit... or foolishness.\n\n"
        "Very well, let me show you the rules here.",
        {},
        {1},
        nullptr,
        -10,
        false,
        Ending::NONE
    };
    m_storyNodes[2] = node2;

    // Node 3: After first riddle
    StoryNode* node3 = new StoryNode{
        3,
        "The void shifts around you. You sense you're being tested for something greater.",
        "Impressive. But that was merely a warmup.\n\n"
        "Let me show you what real challenges look like.",
        {"Bring it on", "Can we talk about this?"},
        {4, 5},
        nullptr,
        0,
        false,
        Ending::NONE
    };
    m_storyNodes[3] = node3;

    // Node 4: Brave path - Cipher challenge
    StoryNode* node4 = new StoryNode{
        4,
        "The Game Master nods approvingly.",
        "Courage. I respect that. Here's a coded message - decrypt it if you can.",
        {},
        {6},
        m_riddles[1],
        0,
        false,
        Ending::NONE
    };
    m_storyNodes[4] = node4;

    // Node 5: Negotiation attempt
    StoryNode* node5 = new StoryNode{
        5,
        "The shadows around you tighten.",
        "Talk? There is no negotiation here, only survival.\n\n"
        "Though... if you prove yourself, perhaps we can make a deal.",
        {"Fine, give me the next riddle", "A deal? Tell me more"},
        {4, 10},
        nullptr,
        0,
        false,
        Ending::NONE
    };
    m_storyNodes[5] = node5;

    // Node 6: After cipher - Logic puzzle
    StoryNode* node6 = new StoryNode{
        6,
        "The message was clear: 'THIS IS NOT FREE'. Your escape won't be easy.",
        "You're sharper than most. Now for a true test of logic.",
        {},
        {7},
        m_riddles[2],
        0,
        false,
        Ending::NONE
    };
    m_storyNodes[6] = node6;

    // Node 7: Critical decision point
    StoryNode* node7 = new StoryNode{
        7,
        "The Game Master seems impressed, yet conflicted.",
        "You've proven yourself worthy. Few make it this far.\n\n"
        "I offer you a choice: Continue fighting for freedom, or join me. Become my apprentice and share in eternal power.",
        {"I want freedom, nothing else", "Tell me about this power", "I'll keep solving your riddles"},
        {8, 11, 8},
        nullptr,
        0,
        false,
        Ending::NONE
    };
    m_storyNodes[7] = node7;

    // Node 8: Path to freedom - Binary challenge
    StoryNode* node8 = new StoryNode{
        8,
        "The Game Master's expression darkens.",
        "So be it. Let's see if you can decode your way out.",
        {},
        {9},
        m_riddles[3],
        0,
        false,
        Ending::NONE
    };
    m_storyNodes[8] = node8;

    // Node 9: Final riddle before ending
    StoryNode* node9 = new StoryNode{
        9,
        "Light begins to pierce through the darkness. Freedom is close...",
        "One final riddle. Answer correctly, and you're truly free.\n\n"
        "Fail, and you stay here forever.",
        {},
        {12},
        m_riddles[4],
        0,
        false,
        Ending::NONE
    };
    m_storyNodes[9] = node9;

    // Node 10: Deal path
    StoryNode* node10 = new StoryNode{
        10,
        "The Game Master extends a shadowy hand.",
        "As my apprentice, you'll have power over other souls, immortality, and knowledge beyond mortal comprehension.\n\n"
        "But you'll serve me for eternity. Do you accept?",
        {"I accept", "No, I choose freedom"},
        {13, 8},
        nullptr,
        0,
        false,
        Ending::NONE
    };
    m_storyNodes[10] = node10;

    // Node 11: Interest in power (leading to deal)
    StoryNode* node11 = new StoryNode{
        11,
        "The Game Master's eyes gleam with dark satisfaction.",
        "Power eternal, knowledge infinite. You could be my successor one day.\n\n"
        "But first, you must prove absolute loyalty.",
        {"Show me", "Actually, I want freedom"},
        {10, 8},
        nullptr,
        10,
        false,
        Ending::NONE
    };
    m_storyNodes[11] = node11;

    // Node 12: True freedom ending (based on riddle success)
    StoryNode* node12 = new StoryNode{
        12,
        "The letter 'E'. The beginning and end of everything.\n\n"
        "Brilliant light floods the void. The Game Master fades away, defeated by your wit.",
        "You... you've actually done it. Go, then. You've earned your freedom.\n\n"
        "But remember - few escape Death itself. You are truly exceptional.",
        {},
        {},
        nullptr,
        0,
        true,
        Ending::TRUE_FREEDOM
    };
    m_storyNodes[12] = node12;

    // Node 13: The Deal ending
    StoryNode* node13 = new StoryNode{
        13,
        "Your hand touches the Game Master's. Power surges through you, dark and intoxicating.",
        "Welcome, my apprentice. Your mortal life ends, but your immortal one begins.\n\n"
        "Together, we shall test countless souls...",
        {},
        {},
        nullptr,
        0,
        true,
        Ending::THE_DEAL
    };
    m_storyNodes[13] = node13;
}

void Story::startStory() {
    m_currentNodeId = 0;
    m_visitedNodes.clear();
    m_gameEnded = false;
    m_currentEnding = Ending::NONE;
    goToNode(0);
}

const Story::StoryNode* Story::getCurrentNode() const {
    return m_storyNodes.value(m_currentNodeId, nullptr);
}

void Story::makeChoice(int choiceIndex) {
    const StoryNode* currentNode = getCurrentNode();
    if (!currentNode || choiceIndex < 0 || choiceIndex >= currentNode->nextNodeIds.size()) {
        return;
    }

    int nextNodeId = currentNode->nextNodeIds[choiceIndex];
    goToNode(nextNodeId);
}

void Story::goToNode(int nodeId) {
    if (!m_storyNodes.contains(nodeId)) {
        return;
    }

    m_currentNodeId = nodeId;
    m_visitedNodes.append(nodeId);

    const StoryNode* node = m_storyNodes[nodeId];
    
    emit narrativeChanged(node->narrative);
    emit storyProgressed(nodeId);

    if (node->isEnding) {
        m_gameEnded = true;
        m_currentEnding = node->endingType;
        emit endingReached(node->endingType);
    } else if (!node->choices.isEmpty()) {
        emit choicesAvailable(node->choices);
    }
}

bool Story::hasRiddle() const {
    const StoryNode* node = getCurrentNode();
    return node && node->riddle != nullptr;
}

Riddle* Story::getCurrentRiddle() const {
    const StoryNode* node = getCurrentNode();
    return node ? node->riddle : nullptr;
}

void Story::solveRiddle(bool success) {
    const StoryNode* node = getCurrentNode();
    if (!node || !node->riddle) {
        return;
    }

    if (success && !node->nextNodeIds.isEmpty()) {
        goToNode(node->nextNodeIds[0]);
    }
    // If failure, the damage is handled by GameState
}

QString Story::getEndingDescription(Ending ending) const {
    switch (ending) {
        case Ending::TRUE_FREEDOM:
            return "TRUE FREEDOM\n\nYou outsmarted Death itself. Through wit, courage, and determination, "
                   "you've achieved what few mortals ever do - true escape from the Game Master's realm. "
                   "You return to life, forever changed but truly free.";
        
        case Ending::THE_DEAL:
            return "THE DEAL\n\nYou've chosen power over freedom. As the Game Master's apprentice, "
                   "you gain immortality and knowledge beyond mortal comprehension. "
                   "But at what cost? You'll spend eternity testing other souls, becoming what you once feared.";
        
        case Ending::PARTIAL_ESCAPE:
            return "PARTIAL ESCAPE\n\nYou've escaped, but not without sacrifice. "
                   "Part of you remains forever bound to the Game Master's realm. "
                   "You're free, but you'll always feel the shadow watching.";
        
        case Ending::THE_LOOP:
            return "THE LOOP\n\nYou've fallen into the Game Master's trap. "
                   "Everything resets. You'll wake up again in the void, "
                   "memories hazy, doomed to repeat this cycle until you find the true path.";
        
        case Ending::DEATHS_EMBRACE:
            return "DEATH'S EMBRACE\n\nYour health depleted, your will broken. "
                   "The Game Master claims your soul. You become another lost spirit "
                   "trapped in the void, a warning to future challengers.";
        
        default:
            return "Unknown ending...";
    }
}
