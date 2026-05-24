/// @file LetterUtils.cpp
/// @brief 字母与数值转换工具类实现
/// @details 映射关系：A=1, B=2, C=3, D=4, E=5
///          五进制加法：值相加后，若超过5则减5（模5循环到1-5范围）

#include "LetterUtils.h"
#include <random>

static std::mt19937& getRandomEngine() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

int LetterUtils::letterToValue(char letter) {
    if (letter >= 'A' && letter <= 'E') {
        // A=1, B=2, C=3, D=4, E=5
        return (letter - 'A') + 1;
    }
    return -1;
}

char LetterUtils::valueToLetter(int value) {
    // 将任意整数映射到 1-5 范围，然后转为字母 A-E
    // 1→A, 2→B, 3→C, 4→D, 5→E
    // 公式：((value-1) % 5 + 5) % 5 得到 0-4，再加 'A'
    int normalized = ((value - 1) % 5 + 5) % 5;
    return static_cast<char>('A' + normalized);
}

char LetterUtils::addLetters(char a, char b) {
    int valA = letterToValue(a);
    int valB = letterToValue(b);
    // 五进制加法：两值相加后映射回 1-5 范围
    // 例如：A(1)+C(3)=4→D, E(5)+B(2)=7→7≡2→B
    return valueToLetter(valA + valB);
}

bool LetterUtils::isValidLetter(char letter) {
    return letter >= 'A' && letter <= 'E';
}

char LetterUtils::randomLetter() {
    static std::uniform_int_distribution<int> dist(0, 4);
    // 随机生成 A-E（valueToLetter 会将其映射为 1-5 再转字母）
    return valueToLetter(dist(getRandomEngine()) + 1);
}

int LetterUtils::randomNumber() {
    static std::uniform_int_distribution<int> dist(0, 9);
    return dist(getRandomEngine());
}
