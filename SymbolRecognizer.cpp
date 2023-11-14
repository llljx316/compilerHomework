#include "util.h"
#include "SymbolRecognizer.h"
#include <minwindef.h>

extern std::set<std::string> Identifier; // 引用外部定义的标识符集合

// 将一个字符串插入到标识符集合中
// ch: 要插入的字符串
void SymbolRecognizer::addToken(std::string ch)
{
    Identifier.insert(ch); // 将字符串插入到标识符集合
}

// 判断给定字符串的指定部分是否为有效的标识符
// str: 给定的字符串，start: 起始位置的引用，end: 结束位置的引用
// line: 行号的引用，col: 列号的引用
int SymbolRecognizer::isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col)
{
    uint pos = start; // 当前处理的位置
    uint cur_col = col; // 当前列号
    std::string word = ""; // 用于存储识别出的标识符

    // 循环遍历字符串中的字符
    while (1) {
        // 如果字符是字母或数字，将其添加到word中
        if (isAlphaOrUnderscore(str[pos]) || isNumericDigit(str[pos])) {
            word += str[pos];
            pos++;
            cur_col++;
        }
        else {
            break; // 遇到非字母或数字字符时停止循环
        }
    }

    end = pos - 1; // 设置标识符的结束位置
    col = cur_col; // 更新列号

    // 如果未识别出标识符，返回NOT
    if (word == "")
        return NOT;

    // 成功识别标识符，将其添加到标识符集合，并返回SUCCESS
    addToken(word);
    return SUCCESS;
}
