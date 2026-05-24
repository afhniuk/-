/// @file QuestionBank.cpp
/// @brief 题库管理类实现

#include "QuestionBank.h"
#include "LetterUtils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <random>

QuestionBank::QuestionBank(const std::string& dataDir) {
    ensureDataDir(dataDir);
    questionsFilePath = dataDir + "\\questions.txt";
    loadFromFile();

    if (questions.empty()) {
        generateBuiltinQuestions();
        saveToFile();
    }
}

void QuestionBank::ensureDataDir(const std::string& dataDir) {
    std::error_code ec;
    if (!std::filesystem::exists(dataDir)) {
        std::filesystem::create_directories(dataDir, ec);
    }
}

void QuestionBank::loadFromFile() {
    questions.clear();
    std::ifstream file(questionsFilePath);
    if (!file.is_open()) return;

    std::string line;
    Question currentQ;
    int section = 0;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (line[0] == '[') {
            if (currentQ.id > 0) {
                questions.push_back(currentQ);
                currentQ = Question();
            }

            line = line.substr(1, line.length() - 2);
            std::istringstream iss(line);
            std::string token;

            if (std::getline(iss, token, ':')) currentQ.id = std::stoi(token);
            if (std::getline(iss, token, ':')) currentQ.aValue = std::stoi(token);
            if (std::getline(iss, token)) currentQ.description = token;
            section = 1;
        } else if (section == 1) {
            std::istringstream iss(line);
            std::string cell;
            while (iss >> cell) {
                currentQ.gridData.push_back(cell);
            }
        }
    }

    if (currentQ.id > 0) {
        questions.push_back(currentQ);
    }

    file.close();
}

void QuestionBank::saveToFile() {
    std::ofstream file(questionsFilePath);
    if (!file.is_open()) return;

    file << "# 密阵突围 题库文件\n";
    file << "# 格式：[题号:a值:描述]\n";
    file << "# 然后每行列出该题的盘面数据（..=空格，字母数字=代码）\n\n";

    for (const auto& q : questions) {
        file << "[" << q.id << ":" << q.aValue << ":" << q.description << "]\n";

        int size = 5 * q.aValue;
        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                int idx = r * size + c;
                if (idx < static_cast<int>(q.gridData.size())) {
                    file << q.gridData[idx] << " ";
                } else {
                    file << ".. ";
                }
            }
            file << "\n";
        }
        file << "\n";
    }
    file.close();
}

void QuestionBank::addQuestion(const Question& q) {
    int maxId = 0;
    for (const auto& existing : questions) {
        maxId = std::max(maxId, existing.id);
    }

    Question newQ = q;
    newQ.id = maxId + 1;
    questions.push_back(newQ);
    saveToFile();
}

const Question* QuestionBank::getQuestionById(int id) const {
    for (const auto& q : questions) {
        if (q.id == id) return &q;
    }
    return nullptr;
}

void QuestionBank::showQuestionList() const {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║                  题 库 列 表                         ║\n";
    std::cout << "╠════╤══════╤══════════════════════════════════════════╣\n";
    std::cout << "║题号│a 值  │描述                                      ║\n";
    std::cout << "╟────┼──────┼──────────────────────────────────────────╢\n";

    if (questions.empty()) {
        std::cout << "║              暂无题目                                ║\n";
    } else {
        for (const auto& q : questions) {
            std::string desc = q.description;
            if (desc.length() > 40) desc = desc.substr(0, 37) + "...";
            std::cout << "║"
                      << std::setw(3) << q.id << " │"
                      << std::setw(5) << q.aValue << " │"
                      << std::setw(41) << desc << " ║\n";
        }
    }
    std::cout << "╚════╧══════╧══════════════════════════════════════════╝\n\n";
}

void QuestionBank::generateBuiltinQuestions() {
    // 题目1：a=2（10x10盘面）
    {
        Question q;
        q.id = 1;
        q.aValue = 2;
        q.description = "入门关卡 - 10x10盘面";
        int size = 10;
        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                if ((r + c) % 3 == 0) {
                    q.gridData.push_back("..");
                } else {
                    // valueToLetter 使用1-5映射：0→E,1→A,2→B,3→C,4→D
                    char letter = LetterUtils::valueToLetter((r + c) % 5 + 1);
                    int number = (r * c) % 10;
                    q.gridData.push_back(std::string(1, letter) + std::to_string(number));
                }
            }
        }
        questions.push_back(q);
    }

    // 题目2：a=4（20x20盘面）
    {
        Question q;
        q.id = 2;
        q.aValue = 4;
        q.description = "进阶关卡 - 20x20盘面";
        int size = 20;
        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                char letter = LetterUtils::valueToLetter((r * 3 + c * 2) % 5 + 1);
                int number = (r * 7 + c * 3) % 10;
                q.gridData.push_back(std::string(1, letter) + std::to_string(number));
            }
        }
        questions.push_back(q);
    }

    // 题目3：a=6（30x30盘面）
    {
        Question q;
        q.id = 3;
        q.aValue = 6;
        q.description = "困难关卡 - 30x30盘面";
        int size = 30;
        for (int r = 0; r < size; ++r) {
            for (int c = 0; c < size; ++c) {
                char letter = LetterUtils::valueToLetter((r * c) % 5 + 1);
                int number = (r + c * 2) % 10;
                q.gridData.push_back(std::string(1, letter) + std::to_string(number));
            }
        }
        questions.push_back(q);
    }
}

std::string Question::toString() const {
    std::ostringstream oss;
    oss << "题目 #" << id << " (a=" << aValue << ", "
        << (5 * aValue) << "x" << (5 * aValue) << ") - " << description;
    return oss.str();
}
