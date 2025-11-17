#include "DialogueWidget.h"
#include <QScrollArea>

DialogueWidget::DialogueWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
}

void DialogueWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    
    // Narrative label
    m_narrativeLabel = new QLabel(this);
    m_narrativeLabel->setWordWrap(true);
    m_narrativeLabel->setStyleSheet(
        "QLabel {"
        "    background-color: rgba(20, 20, 30, 180);"
        "    color: #E0E0E0;"
        "    padding: 15px;"
        "    border-radius: 5px;"
        "    font-size: 14px;"
        "    font-style: italic;"
        "}"
    );
    m_narrativeLabel->setMinimumHeight(60);
    mainLayout->addWidget(m_narrativeLabel);
    
    // Game Master dialogue
    m_gameMasterLabel = new QLabel(this);
    m_gameMasterLabel->setWordWrap(true);
    m_gameMasterLabel->setStyleSheet(
        "QLabel {"
        "    background-color: rgba(80, 20, 20, 180);"
        "    color: #FFD700;"
        "    padding: 15px;"
        "    border: 2px solid #8B0000;"
        "    border-radius: 5px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
    );
    m_gameMasterLabel->setMinimumHeight(60);
    mainLayout->addWidget(m_gameMasterLabel);
    
    // Riddle widget
    m_riddleWidget = new QWidget(this);
    QVBoxLayout* riddleLayout = new QVBoxLayout(m_riddleWidget);
    
    m_riddleQuestionText = new QTextEdit(this);
    m_riddleQuestionText->setReadOnly(true);
    m_riddleQuestionText->setMaximumHeight(100);
    m_riddleQuestionText->setStyleSheet(
        "QTextEdit {"
        "    background-color: rgba(30, 30, 50, 200);"
        "    color: #FFFFFF;"
        "    border: 2px solid #4169E1;"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "    font-size: 13px;"
        "}"
    );
    riddleLayout->addWidget(m_riddleQuestionText);
    
    m_riddleAnswerInput = new QTextEdit(this);
    m_riddleAnswerInput->setMaximumHeight(40);
    m_riddleAnswerInput->setPlaceholderText("Enter your answer here...");
    m_riddleAnswerInput->setStyleSheet(
        "QTextEdit {"
        "    background-color: #FFFFFF;"
        "    border: 2px solid #4169E1;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "    font-size: 13px;"
        "}"
    );
    riddleLayout->addWidget(m_riddleAnswerInput);
    
    QHBoxLayout* riddleButtonLayout = new QHBoxLayout();
    m_submitButton = new QPushButton("Submit Answer", this);
    m_submitButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4169E1;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #5179F1;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3159D1;"
        "}"
    );
    connect(m_submitButton, &QPushButton::clicked, this, [this]() {
        emit answerSubmitted(m_riddleAnswerInput->toPlainText());
    });
    riddleButtonLayout->addWidget(m_submitButton);
    
    m_hintButton = new QPushButton("Request Hint (-5 HP)", this);
    m_hintButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #FF8C00;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #FF9C10;"
        "}"
    );
    connect(m_hintButton, &QPushButton::clicked, this, &DialogueWidget::hintRequested);
    riddleButtonLayout->addWidget(m_hintButton);
    
    riddleLayout->addLayout(riddleButtonLayout);
    
    m_feedbackLabel = new QLabel(this);
    m_feedbackLabel->setWordWrap(true);
    m_feedbackLabel->setAlignment(Qt::AlignCenter);
    m_feedbackLabel->setMinimumHeight(30);
    riddleLayout->addWidget(m_feedbackLabel);
    
    m_riddleWidget->setVisible(false);
    mainLayout->addWidget(m_riddleWidget);
    
    // Choices widget
    m_choicesWidget = new QWidget(this);
    m_choicesLayout = new QVBoxLayout(m_choicesWidget);
    m_choicesLayout->setSpacing(10);
    mainLayout->addWidget(m_choicesWidget);
    
    mainLayout->addStretch();
}

void DialogueWidget::setNarrative(const QString& text) {
    m_narrativeLabel->setText(text);
}

void DialogueWidget::setGameMasterDialogue(const QString& text) {
    m_gameMasterLabel->setText("Game Master: " + text);
}

void DialogueWidget::setChoices(const QVector<QString>& choices) {
    clearChoices();
    
    for (int i = 0; i < choices.size(); ++i) {
        QPushButton* button = new QPushButton(choices[i], this);
        button->setStyleSheet(
            "QPushButton {"
            "    background-color: rgba(60, 60, 80, 200);"
            "    color: white;"
            "    border: 2px solid #4169E1;"
            "    border-radius: 5px;"
            "    padding: 12px;"
            "    text-align: left;"
            "    font-size: 13px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgba(70, 70, 100, 220);"
            "    border-color: #5179F1;"
            "}"
            "QPushButton:pressed {"
            "    background-color: rgba(50, 50, 70, 200);"
            "}"
        );
        
        connect(button, &QPushButton::clicked, this, [this, i]() {
            emit choiceSelected(i);
        });
        
        m_choicesLayout->addWidget(button);
        m_choiceButtons.append(button);
    }
}

void DialogueWidget::clearChoices() {
    for (QPushButton* button : m_choiceButtons) {
        button->deleteLater();
    }
    m_choiceButtons.clear();
}

void DialogueWidget::setRiddleMode(bool enabled) {
    m_riddleWidget->setVisible(enabled);
    m_choicesWidget->setVisible(!enabled);
    m_feedbackLabel->clear();
    
    if (enabled) {
        m_riddleAnswerInput->setFocus();
    }
}

void DialogueWidget::setRiddleQuestion(const QString& question) {
    m_riddleQuestionText->setText(question);
}

QString DialogueWidget::getRiddleAnswer() const {
    return m_riddleAnswerInput->toPlainText().trimmed();
}

void DialogueWidget::clearRiddleAnswer() {
    m_riddleAnswerInput->clear();
    m_feedbackLabel->clear();
}

void DialogueWidget::showHint(const QString& hint) {
    m_feedbackLabel->setStyleSheet(
        "QLabel {"
        "    background-color: rgba(255, 140, 0, 180);"
        "    color: white;"
        "    padding: 10px;"
        "    border-radius: 5px;"
        "    font-weight: bold;"
        "}"
    );
    m_feedbackLabel->setText("💡 Hint: " + hint);
}

void DialogueWidget::showFeedback(const QString& feedback, bool success) {
    if (success) {
        m_feedbackLabel->setStyleSheet(
            "QLabel {"
            "    background-color: rgba(50, 200, 50, 180);"
            "    color: white;"
            "    padding: 10px;"
            "    border-radius: 5px;"
            "    font-weight: bold;"
            "}"
        );
        m_feedbackLabel->setText("✓ Correct! " + feedback);
    } else {
        m_feedbackLabel->setStyleSheet(
            "QLabel {"
            "    background-color: rgba(200, 50, 50, 180);"
            "    color: white;"
            "    padding: 10px;"
            "    border-radius: 5px;"
            "    font-weight: bold;"
            "}"
        );
        m_feedbackLabel->setText("✗ Wrong! " + feedback);
    }
}
