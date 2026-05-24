/// @file Grid.h
/// @brief 格子类声明，表示盘面上的一个单元格

#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <string>

class Grid {
public:
    char letter;    ///< 字母，取值范围 A-E
    int number;     ///< 数字，取值范围 0-9
    bool isEmpty;   ///< 是否为空格子

    Grid();

    Grid(char l, int n);

    /// @brief 生成随机代码的格子
    static Grid random();

    /// @brief 获取格子的字符串表示，如"[A3]"，空格子返回"[  ]"
    std::string toString() const;

    /// @brief 判断两个格子是否具有相同的字母
    bool hasSameLetter(const Grid& other) const;

    /// @brief 判断两个格子是否具有相同的数字
    bool hasSameNumber(const Grid& other) const;

    /// @brief 判断两个格子是否完全相同（字母和数字都相同）
    bool isIdentical(const Grid& other) const;

    /// @brief 清空格子
    void clear();

    friend std::ostream& operator<<(std::ostream& os, const Grid& grid);
};

#endif // GRID_H
