#include "common.h"
#include "util.h"
#include "OperatorDetector.h"
#include "LexicalAnalyzer.h"

extern std::string Operator[OPERATOR_NUM]; // 引用在其他地方定义的运算符数组

// 构造函数：初始化运算符映射表
OperatorDetector::OperatorDetector()
{
    // 将所有运算符添加到映射表中，映射它们的索引
    for (unsigned int i = 0; i < OPERATOR_NUM; i++)
        Operator_Map[Operator[i]] = i;
}

// 判断给定字符串的指定位置是否是一个有效的运算符
// str: 给定的字符串，start: 起始位置的引用，end: 结束位置的引用
// line: 行号的引用，col: 列号的引用
int OperatorDetector::isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) {
    char ch = str[start]; // 当前处理的字符

    // 根据字符判断运算符类型
    switch (ch) {
        // 单字符运算符，直接设置end为start
    case ',':
    case '.':
    case '?':
    case ':':
        end = start;
        break;

        // 可能是双字符运算符的情况，需要检查下一个字符
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '=':
    case '!':
    case '~':
    case '^':
    case '>':
    case '<':
    case '&':
    case '|':
        // 如果下一个字符是 '=' 或者相同字符（不包括 '>' 和 '<'），则是双字符运算符
        end = (str[start + 1] == '=' || (ch == str[start + 1] && ch != '>' && ch != '<')) ? start + 1 : start;
        break;

        // 如果不是上述情况，则不是运算符
    default:
        return NOT;
    }

    // 更新列号
    col += (end - start) + 1;
    // 返回SUCCESS表示成功识别出运算符
    return SUCCESS;
}
