#ifndef DIALOGUEWIDGET_H
#define DIALOGUEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>

class DialogueWidget : public QWidget {
    Q_OBJECT

public:
    explicit DialogueWidget(QWidget* parent = nullptr);
    
    void setNarrative(const QString& text);
    void setGameMasterDialogue(const QString& text);
    void setChoices(const QVector<QString>& choices);
    void clearChoices();
    
    void setRiddleMode(bool enabled);
    void setRiddleQuestion(const QString& question);
    QString getRiddleAnswer() const;
    void clearRiddleAnswer();
    
    void showHint(const QString& hint);
    void showFeedback(const QString& feedback, bool success);

signals:
    void choiceSelected(int choiceIndex);
    void answerSubmitted(QString answer);
    void hintRequested();

private:
    void setupUI();
    
    QLabel* m_narrativeLabel;
    QLabel* m_gameMasterLabel;
    QTextEdit* m_riddleQuestionText;
    QTextEdit* m_riddleAnswerInput;
    QPushButton* m_submitButton;
    QPushButton* m_hintButton;
    QLabel* m_feedbackLabel;
    QVBoxLayout* m_choicesLayout;
    QWidget* m_choicesWidget;
    QWidget* m_riddleWidget;
    QVector<QPushButton*> m_choiceButtons;
};

#endif // DIALOGUEWIDGET_H
