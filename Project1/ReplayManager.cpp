/// @file ReplayManager.cpp
/// @brief 回放管理类实现

#include "ReplayManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <fstream>

ReplayManager::ReplayManager(const std::string& dataDir) {
    ensureDataDir(dataDir);
    replayFilePath = dataDir + "\\replay.txt";
    loadReplays();
}

void ReplayManager::ensureDataDir(const std::string& dataDir) {
    std::error_code ec;
    if (!std::filesystem::exists(dataDir)) {
        std::filesystem::create_directories(dataDir, ec);
    }
}

void ReplayManager::recordStep(ReplayRecord& record, Direction dir,
                                MoveResult result, int mergeCount) {
    ReplayStep step;
    step.stepNumber = static_cast<int>(record.steps.size()) + 1;
    step.direction = dir;
    step.result = result;
    step.mergeCount = mergeCount;
    record.steps.push_back(step);
}

void ReplayManager::saveReplay(const ReplayRecord& record) {
    ReplayRecord rec = record;
    rec.finalMergeCount = record.steps.empty() ? 0 : record.steps.back().mergeCount;
    rec.totalSteps = static_cast<int>(record.steps.size());

    std::ofstream file(replayFilePath, std::ios::app);
    if (!file.is_open()) return;

    file << "=====REPLAY=====\n";
    file << rec.toHeaderString() << "\n";

    file << "BOARD\n";
    for (const auto& cell : rec.initialBoard) {
        file << cell << " ";
    }
    file << "\n";

    file << "CODES\n";
    for (size_t i = 0; i < rec.initialCodes.size(); ++i) {
        if (i > 0) file << ",";
        file << rec.initialCodes[i].row << "-" << rec.initialCodes[i].col;
    }
    file << "\n";

    file << "STEPS\n";
    for (const auto& step : rec.steps) {
        file << step.stepNumber << ":"
             << directionToChar(step.direction) << ":"
             << (step.result == MoveResult::SUCCESS_MERGE ? "M" :
                 step.result == MoveResult::SUCCESS_MOVE ? "V" : "F") << ":"
             << step.mergeCount << "\n";
    }
    file << "=====END=====\n\n";
    file.close();

    loadReplays();
}

void ReplayManager::loadReplays() {
    replays.clear();
    std::ifstream file(replayFilePath);
    if (!file.is_open()) return;

    std::string line;
    ReplayRecord currentRec;
    int section = 0;

    while (std::getline(file, line)) {
        if (line == "=====REPLAY=====") {
            currentRec = ReplayRecord();
            section = 1;
        } else if (line == "=====END=====") {
            if (!currentRec.playerName.empty()) {
                replays.push_back(currentRec);
            }
            section = 0;
        } else if (section == 1 && line.substr(0, 4) == "NAME") {
            std::istringstream iss(line);
            std::string token;
            std::getline(iss, token, ':');
            std::getline(iss, currentRec.playerName, ':');
            std::getline(iss, token, ':');
            currentRec.aValue = std::stoi(token);
            std::getline(iss, token);
            section = 2;
        } else if (section == 2 && line.substr(0, 5) == "BOARD") {
            // 等待数据
        } else if (section == 2 && !line.empty() && line[0] != 'C') {
            std::istringstream iss(line);
            std::string cell;
            while (iss >> cell) {
                currentRec.initialBoard.push_back(cell);
            }
        } else if (line.substr(0, 5) == "CODES") {
            section = 3;
        } else if (section == 3 && !line.empty() && line[0] != 'S') {
            std::istringstream iss(line);
            std::string token;
            while (std::getline(iss, token, ',')) {
                auto dashPos = token.find('-');
                if (dashPos != std::string::npos) {
                    int r = std::stoi(token.substr(0, dashPos));
                    int c = std::stoi(token.substr(dashPos + 1));
                    currentRec.initialCodes.push_back(Position(r, c));
                }
            }
        } else if (line.substr(0, 5) == "STEPS") {
            section = 4;
        } else if (section == 4 && !line.empty() && line[0] != '=') {
            std::istringstream iss(line);
            std::string token;
            ReplayStep step;
            if (std::getline(iss, token, ':')) step.stepNumber = std::stoi(token);
            if (std::getline(iss, token, ':')) step.direction = charToDirection(token[0]);
            if (std::getline(iss, token, ':')) {
                step.result = (token[0] == 'M') ? MoveResult::SUCCESS_MERGE :
                              (token[0] == 'V') ? MoveResult::SUCCESS_MOVE :
                              MoveResult::FAILED;
            }
            if (std::getline(iss, token)) step.mergeCount = std::stoi(token);
            currentRec.steps.push_back(step);
        }
    }

    file.close();
}

void ReplayManager::showReplayList() const {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                    回 放 列 表                               ║\n";
    std::cout << "╠════╤══════════════╤══════╤══════╤════════╤══════════════════╣\n";
    std::cout << "║序号│玩家名        │a 值  │总步数│最终合并│得分率            ║\n";
    std::cout << "╟────┼──────────────┼──────┼──────┼────────┼──────────────────╢\n";

    if (replays.empty()) {
        std::cout << "║                   暂无回放记录                              ║\n";
    } else {
        for (size_t i = 0; i < replays.size(); ++i) {
            const auto& rec = replays[i];
            double rate = (rec.totalSteps > 0) ?
                          static_cast<double>(rec.finalMergeCount) / rec.totalSteps : 0.0;
            std::cout << "║"
                      << std::setw(3) << (i + 1) << " │"
                      << std::setw(13) << rec.playerName.substr(0, 13) << "│"
                      << std::setw(5) << rec.aValue << " │"
                      << std::setw(5) << rec.totalSteps << " │"
                      << std::setw(7) << rec.finalMergeCount << " │"
                      << std::setw(17) << std::fixed << std::setprecision(3)
                      << rate << " ║\n";
        }
    }
    std::cout << "╚════╧══════════════╧══════╧══════╧════════╧══════════════════╝\n\n";
}

const ReplayRecord* ReplayManager::getReplayByIndex(int index) const {
    if (index >= 0 && index < static_cast<int>(replays.size())) {
        return &replays[index];
    }
    return nullptr;
}

std::string ReplayStep::toString() const {
    std::ostringstream oss;
    oss << "步骤 " << stepNumber << ": "
        << directionToString(direction) << " → "
        << (result == MoveResult::SUCCESS_MERGE ? "合并成功" :
            result == MoveResult::SUCCESS_MOVE ? "移动成功" : "失败")
        << " (累计合并: " << mergeCount << ")";
    return oss.str();
}

std::string ReplayRecord::toHeaderString() const {
    std::ostringstream oss;
    oss << "NAME:" << playerName << ":" << aValue << ":" << "DATE";
    return oss.str();
}
