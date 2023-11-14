// 保留字识别相关的代码
#include "KeywordDetector.h"
#include <vector>
#include "LexicalAnalyzer.h"

extern std::string ReserveWord[RESERVED_WORD_NUM]; // 引用外部定义的保留字数组

// 构造函数：初始化保留字映射表
KeywordDetector::KeywordDetector()
{
    // 遍历保留字数组，建立保留字与其索引的映射关系
    for (unsigned int i = 0; i < RESERVED_WORD_NUM; i++)
        ReservedWord_Map[ReserveWord[i]] = i;
}

// 判断给定字符串的指定部分是否为保留字
// str: 给定的字符串，start: 起始位置的引用，end: 结束位置的引用
// line: 行号的引用，col: 列号的引用
int KeywordDetector::isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col)
{
    uint pos = start; // 当前处理的位置
    uint cur_col = col; // 当前列号
    std::string word = ""; // 用于存储识别出的单词

    // 循环遍历字符串中的字符
    while (1) {
        // 如果字符是字母，将其添加到word中
        if (isAlphaOrUnderscore(str[pos])) {
            word += str[pos];
            pos++;
            cur_col++;
        }
        else {
            break; // 遇到非字母字符时停止循环
        }
    }

    // 在保留字映射表中查找识别出的单词
    auto iter = ReservedWord_Map.find(word);
    if (iter != ReservedWord_Map.end()) { // 如果找到
        end = pos - 1; // 设置结束位置
        col = cur_col; // 更新列号
        return iter->second; // 返回保留字在映射表中的位置
    }
    else {
        return NOT; // 如果未找到，返回NOT
    }
}
