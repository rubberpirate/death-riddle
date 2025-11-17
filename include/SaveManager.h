#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QObject>
#include <QString>
#include <QJsonObject>

class GameState;

class SaveManager : public QObject {
    Q_OBJECT

public:
    explicit SaveManager(QObject* parent = nullptr);
    virtual ~SaveManager() = default;

    bool saveGame(const GameState* gameState, const QString& saveName = "autosave");
    bool loadGame(GameState* gameState, const QString& saveName = "autosave");
    
    QStringList getSaveFiles() const;
    bool deleteSave(const QString& saveName);
    
    QString getSavePath(const QString& saveName) const;

private:
    QJsonObject serializeGameState(const GameState* gameState) const;
    bool deserializeGameState(GameState* gameState, const QJsonObject& json);
    
    QString m_saveDirectory;
};

#endif // SAVEMANAGER_H
