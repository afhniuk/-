/// @file ScoreManager.cpp
/// @brief 成绩管理类实现

#include "ScoreManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <filesystem>

ScoreManager::ScoreManager(const std::string& dataDir) {
    ensureDataDir(dataDir);
    scoresFilePath = dataDir + "\\scores.txt";
    loadFromFile();
}

void ScoreManager::ensureDataDir(const std::string& dataDir) {
    std::error_code ec;
    if (!std::filesystem::exists(dataDir)) {
        std::filesystem::create_directories(dataDir, ec);
    }
}

void ScoreManager::addRecord(const ScoreRecord& record) {
    ScoreRecord rec = record;
    rec.calculateScoreRate();
    records.push_back(rec);
    saveToFile();
}

void ScoreManager::loadFromFile() {
    records.clear();
    std::ifstream file(scoresFilePath);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        ScoreRecord rec;
        std::string temp;

        if (std::getline(iss, rec.playerName, '|') &&
            std::getline(iss, temp, '|')) {
            rec.aValue = std::stoi(temp);
            if (std::getline(iss, temp, '|')) {
                rec.totalTimeSeconds = std::stoi(temp);
                if (std::getline(iss, temp, '|')) {
                    rec.mergeCount = std::stoi(temp);
                    if (std::getline(iss, temp, '|')) {
                        rec.totalMoves = std::stoi(temp);
                        rec.calculateScoreRate();
                        records.push_back(rec);
                    }
                }
            }
        }
    }
    file.close();
}

void ScoreManager::saveToFile() {
    std::ofstream file(scoresFilePath);
    if (!file.is_open()) return;

    file << "# 密阵突围 成绩排行榜\n";
    file << "# 格式：玩家名|a值|用时(秒)|合并次数|总操作数|得分率\n";

    for (const auto& rec : records) {
        file << rec.playerName << "|"
             << rec.aValue << "|"
             << rec.totalTimeSeconds << "|"
             << rec.mergeCount << "|"
             << rec.totalMoves << "|"
             << std::fixed << std::setprecision(3) << rec.scoreRate << "\n";
    }
    file.close();
}

void ScoreManager::showLeaderboard(int filterByA) const {
    std::vector<ScoreRecord> filtered;
    for (const auto& rec : records) {
        if (filterByA < 0 || rec.aValue == filterByA) {
            filtered.push_back(rec);
        }
    }

    std::sort(filtered.begin(), filtered.end(),
              [](const ScoreRecord& a, const ScoreRecord& b) {
                  return a.scoreRate > b.scoreRate;
              });

    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    if (filterByA > 0) {
        std::cout << "║              排行榜 (a = " << filterByA << ")                              ║\n";
    } else {
        std::cout << "║              排行榜 (全部)                                    ║\n";
    }
    std::cout << "╠════╤══════════════╤══════╤══════╤══════╤════════╤══════════╣\n";
    std::cout << "║排名│玩家名        │a 值  │用时(s)│合并  │总操作 │得分率    ║\n";
    std::cout << "╟────┼──────────────┼──────┼──────┼──────┼────────┼──────────╢\n";

    if (filtered.empty()) {
        std::cout << "║                   暂无成绩记录                              ║\n";
    } else {
        int rank = 0;
        for (size_t i = 0; i < filtered.size() && i < 20; ++i) {
            if (i == 0 || filtered[i].scoreRate < filtered[i - 1].scoreRate) {
                rank = static_cast<int>(i) + 1;
            }
            const auto& rec = filtered[i];
            std::cout << "║"
                      << std::setw(3) << rank << " │"
                      << std::setw(13) << rec.playerName.substr(0, 13) << "│"
                      << std::setw(5) << rec.aValue << " │"
                      << std::setw(5) << rec.totalTimeSeconds << " │"
                      << std::setw(5) << rec.mergeCount << " │"
                      << std::setw(7) << rec.totalMoves << " │"
                      << std::setw(9) << std::fixed << std::setprecision(3)
                      << rec.scoreRate << " ║\n";
        }
    }
    std::cout << "╚════╧══════════════╧══════╧══════╧══════╧════════╧══════════╝\n";
    std::cout << "\n得分率 = 合并次数 / 总操作次数\n\n";
}

std::string ScoreRecord::toString() const {
    std::ostringstream oss;
    oss << "玩家：" << playerName
        << " | a=" << aValue
        << " | 用时：" << totalTimeSeconds << "秒"
        << " | 合并：" << mergeCount
        << " | 总操作：" << totalMoves
        << " | 得分率：" << std::fixed << std::setprecision(3) << scoreRate;
    return oss.str();
}
