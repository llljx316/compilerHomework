#include "common.h"
#include "DelimiterDetector.h"

extern std::string Delimiter[DELIMITER_NUM]; // 引用在其他地方定义的分隔符数组

// 在分隔符数组中查找给定的字符串
// ch: 要查找的字符串
int DelimiterDetector::findDelimiter(std::string ch)
{
    // 遍历分隔符数组
    for (int i = 0; i < DELIMITER_NUM; i++) {
        // 如果找到匹配的分隔符，返回其在数组中的索引
        if (ch.find(Delimiter[i]) != -1)
            return i;
    }
    // 如果没有找到匹配的分隔符，返回NOT
    return NOT;
}

// 判断给定字符串的指定位置是否是一个有效的分隔符
// str: 给定的字符串，start: 起始位置的引用，end: 结束位置的引用
// line: 行号的引用，col: 列号的引用
int DelimiterDetector::isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col)
{
    uint pos = start; // 当前处理的位置

    // 调用findDelimiter函数查找当前位置的字符是否是分隔符
    if (findDelimiter(&str[pos]) == NOT) {
        // 如果不是分隔符，返回NOT
        return NOT;
    }
    else {
        // 如果是分隔符，更新start和end位置，并且列号加一
        start = end = pos;
        col++;
        // 返回SUCCESS表示成功找到分隔符
        return SUCCESS;
    }
}
