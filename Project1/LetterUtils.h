/// @file LetterUtils.h
/// @brief 字母与数值转换工具类声明
/// @details 提供字母A-E与数值1-5之间的双向转换，以及五进制加法运算。
///          映射关系：A=1, B=2, C=3, D=4, E=5
///          五进制加法：两字母对应数值相加，结果映射回1-5范围（模5）。
///          例如：A(1) + C(3) = 4 → D

#ifndef LETTERUTILS_H
#define LETTERUTILS_H

/// @brief 字母工具类，提供静态方法进行字母与数值的转换
class LetterUtils {
public:
    /// @brief 将字母转换为对应的数值
    /// @param letter 字母（A-E）
    /// @return 对应的数值：A=1, B=2, C=3, D=4, E=5；若输入非法则返回-1
    static int letterToValue(char letter);

    /// @brief 将数值转换为对应的字母（1-5范围循环）
    /// @param value 数值，自动映射到1-5范围
    /// @return 对应的字母（A-E）：1→A, 2→B, 3→C, 4→D, 5→E
    static char valueToLetter(int value);

    /// @brief 执行两个字母的五进制加法
    /// @param a 第一个字母
    /// @param b 第二个字母
    /// @return 加法结果对应的字母（A-E）
    /// @details 例如：A(1)+C(3)=4→D, E(5)+B(2)=7→7≡2(mod5)→B
    static char addLetters(char a, char b);

    /// @brief 验证字母是否在有效范围内（A-E）
    static bool isValidLetter(char letter);

    /// @brief 随机生成一个有效字母（A-E）
    static char randomLetter();

    /// @brief 随机生成一个数字（0-9）
    static int randomNumber();
};

#endif // LETTERUTILS_H
