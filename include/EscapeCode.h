#ifndef ESCAPE_CODE_H
#define ESCAPE_CODE_H

#include <string>

// 30 黑
// 31 红
// 32 绿
// 33 黄
// 34 蓝
// 35 洋红
// 36 青
// 37 白
#define COLOR_BLACK   "30"
#define COLOR_RED     "31"
#define COLOR_GREEN   "32"
#define COLOR_YELLOW  "33"
#define COLOR_BLUE    "34"
#define COLOR_MAGENTA "35"
#define COLOR_CYAN    "36"
#define COLOR_WHITE   "37"

// 输出不同颜色的字符串
// str: string 原始字符串
// c: string 表示需要的颜色
std::string set_color(std::string str, std::string c);

#endif