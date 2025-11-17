#include "SaveManager.h"
#include "GameState.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QStandardPaths>

SaveManager::SaveManager(QObject* parent)
    : QObject(parent)
{
    m_saveDirectory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/DeathRiddle/saves";
    QDir dir;
    dir.mkpath(m_saveDirectory);
}

bool SaveManager::saveGame(const GameState* gameState, const QString& saveName) {
    QJsonObject json = serializeGameState(gameState);
    
    QJsonDocument doc(json);
    QString filePath = getSavePath(saveName);
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    file.write(doc.toJson());
    file.close();
    return true;
}

bool SaveManager::loadGame(GameState* gameState, const QString& saveName) {
    QString filePath = getSavePath(saveName);
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        return false;
    }
    
    return deserializeGameState(gameState, doc.object());
}

QStringList SaveManager::getSaveFiles() const {
    QDir dir(m_saveDirectory);
    QStringList filters;
    filters << "*.json";
    
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
    QStringList saveNames;
    
    for (const QFileInfo& info : files) {
        saveNames.append(info.baseName());
    }
    
    return saveNames;
}

bool SaveManager::deleteSave(const QString& saveName) {
    QString filePath = getSavePath(saveName);
    return QFile::remove(filePath);
}

QString SaveManager::getSavePath(const QString& saveName) const {
    return m_saveDirectory + "/" + saveName + ".json";
}

QJsonObject SaveManager::serializeGameState(const GameState* gameState) const {
    QJsonObject json;
    
    // Player data
    const Player* player = gameState->getPlayer();
    QJsonObject playerJson;
    playerJson["health"] = player->getHealth();
    playerJson["score"] = player->getScore();
    playerJson["riddlesSolved"] = player->getRiddlesSolved();
    playerJson["failedAttempts"] = player->getFailedAttempts();
    playerJson["name"] = player->getName();
    json["player"] = playerJson;
    
    // Story progress
    const Story* story = gameState->getStory();
    QJsonObject storyJson;
    storyJson["currentNodeId"] = story->getCurrentNodeId();
    
    QJsonArray visitedNodesArray;
    for (int nodeId : story->getVisitedNodes()) {
        visitedNodesArray.append(nodeId);
    }
    storyJson["visitedNodes"] = visitedNodesArray;
    json["story"] = storyJson;
    
    // Game state
    json["hintsRemaining"] = gameState->getHintsRemaining();
    json["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    
    return json;
}

bool SaveManager::deserializeGameState(GameState* gameState, const QJsonObject& json) {
    if (!json.contains("player") || !json.contains("story")) {
        return false;
    }
    
    // Restore player data
    QJsonObject playerJson = json["player"].toObject();
    Player* player = gameState->getPlayer();
    player->setHealth(playerJson["health"].toInt());
    player->addScore(playerJson["score"].toInt() - player->getScore());
    player->setName(playerJson["name"].toString());
    
    // Note: Full story restoration would require more complex state management
    // This is a basic implementation
    
    return true;
}
