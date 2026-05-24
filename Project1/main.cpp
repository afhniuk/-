/// @file main.cpp
/// @brief 《密阵突围》游戏程序入口
/// @details 基于《最强大脑》第13季经典挑战项目，使用C++17面向对象编程实现。
///          合并规则之数字相同：字母按五进制(1-5)相加，数字不变。
///          映射关系：A=1, B=2, C=3, D=4, E=5。
///          例如：A3+C3→D3（A=1,C=3,1+3=4→D）
/// @author 欧典松
/// @date 2026-05

#if defined(_MSC_VER) && !defined(__clang__)
#pragma execution_character_set("utf-8")
#endif
#include "Game.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <stdexcept>

std::string getDataDirectory() {
    std::error_code ec;
    std::filesystem::path currentPath = std::filesystem::current_path(ec);
    if (ec) {
        return "data";
    }

    std::filesystem::path dataPath = currentPath / "data";

    if (!std::filesystem::exists(dataPath)) {
        std::filesystem::create_directories(dataPath, ec);
        if (ec) {
            std::cerr << "警告：无法创建数据目录 " << dataPath.string() << "\n";
            return "data";
        }
    }

    return dataPath.string();
}

int main() {
    try {
        std::string dataDir = getDataDirectory();
        std::cout << "数据存储目录：" << dataDir << "\n";

        Game game(dataDir);
        game.run();

    } catch (const std::exception& e) {
        std::cerr << "程序发生异常：" << e.what() << "\n";
        std::cerr << "按回车键退出...";
        std::cin.get();
        return 1;
    }

    return 0;
}
