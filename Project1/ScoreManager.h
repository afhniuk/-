/// @file ScoreManager.h
/// @brief 成绩管理类声明

#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <string>
#include <vector>
#include <fstream>

struct ScoreRecord {
    std::string playerName;
    int aValue;
    int totalTimeSeconds;
    int mergeCount;
    int totalMoves;
    double scoreRate;

    void calculateScoreRate() {
        scoreRate = (totalMoves > 0) ?
                    static_cast<double>(mergeCount) / totalMoves : 0.0;
    }

    std::string toString() const;
};

class ScoreManager {
private:
    std::string scoresFilePath;
    std::vector<ScoreRecord> records;

public:
    explicit ScoreManager(const std::string& dataDir);

    void addRecord(const ScoreRecord& record);
    void loadFromFile();
    void saveToFile();

    const std::vector<ScoreRecord>& getRecords() const { return records; }

    void showLeaderboard(int filterByA = -1) const;

private:
    void ensureDataDir(const std::string& dataDir);
};

#endif // SCOREMANAGER_H
