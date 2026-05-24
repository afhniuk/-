/// @file Grid.cpp
/// @brief 格子类实现

#include "Grid.h"
#include "LetterUtils.h"
#include <iomanip>
#include <sstream>

Grid::Grid() : letter('A'), number(0), isEmpty(true) {}

Grid::Grid(char l, int n) : letter(l), number(n), isEmpty(false) {}

Grid Grid::random() {
    Grid g;
    g.letter = LetterUtils::randomLetter();
    g.number = LetterUtils::randomNumber();
    g.isEmpty = false;
    return g;
}

std::string Grid::toString() const {
    if (isEmpty) {
        return "[  ]";
    }
    std::ostringstream oss;
    oss << "[" << letter << number << "]";
    return oss.str();
}

bool Grid::hasSameLetter(const Grid& other) const {
    return !isEmpty && !other.isEmpty && letter == other.letter;
}

bool Grid::hasSameNumber(const Grid& other) const {
    return !isEmpty && !other.isEmpty && number == other.number;
}

bool Grid::isIdentical(const Grid& other) const {
    return !isEmpty && !other.isEmpty
           && letter == other.letter
           && number == other.number;
}

void Grid::clear() {
    isEmpty = true;
    letter = 'A';
    number = 0;
}

std::ostream& operator<<(std::ostream& os, const Grid& grid) {
    os << grid.toString();
    return os;
}
