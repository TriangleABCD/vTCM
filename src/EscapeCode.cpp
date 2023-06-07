#include "EscapeCode.h"

// 输出不同颜色的字符串
// str: string 原始字符串
// c: string 表示需要的颜色
std::string set_color(std::string str, std::string c) {
	std::string result = "\033[";
	result += c;
	result += "m ";
	result += str;
	return result;
}