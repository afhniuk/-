/// @file Types.h
/// @brief 《密阵突围》项目的基础类型定义
/// @author 欧典松
/// @date 2026-05

#ifndef TYPES_H
#define TYPES_H

#include <string>

/// @brief 移动方向枚举
enum class Direction {
    UP,     ///< 向上移动
    DOWN,   ///< 向下移动
    LEFT,   ///< 向左移动
    RIGHT   ///< 向右移动
};

/// @brief 移动结果枚举
enum class MoveResult {
    SUCCESS_MERGE,  ///< 成功合并（两个代码合并为一个）
    SUCCESS_MOVE,   ///< 成功移动（仅移动到空位，未合并）
    FAILED          ///< 移动失败（目标不可达或无法合并）
};

/// @brief 盘面坐标结构体
struct Position {
    int row;    ///< 行索引（0-based）
    int col;    ///< 列索引（0-based）

    Position() : row(-1), col(-1) {}

    Position(int r, int c) : row(r), col(c) {}

    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

    bool isValid() const { return row >= 0 && col >= 0; }
};

/// @brief 将方向转换为字符串表示
inline std::string directionToString(Direction dir) {
    switch (dir) {
        case Direction::UP:    return "上";
        case Direction::DOWN:  return "下";
        case Direction::LEFT:  return "左";
        case Direction::RIGHT: return "右";
        default:               return "未知";
    }
}

/// @brief 将方向转换为单字符（用于回放存储）
inline char directionToChar(Direction dir) {
    switch (dir) {
        case Direction::UP:    return 'U';
        case Direction::DOWN:  return 'D';
        case Direction::LEFT:  return 'L';
        case Direction::RIGHT: return 'R';
        default:               return '?';
    }
}

/// @brief 将单字符转换为方向
inline Direction charToDirection(char ch) {
    switch (ch) {
        case 'U': case 'u': return Direction::UP;
        case 'D': case 'd': return Direction::DOWN;
        case 'L': case 'l': return Direction::LEFT;
        case 'R': case 'r': return Direction::RIGHT;
        default:            return Direction::UP;
    }
}

#endif // TYPES_H
