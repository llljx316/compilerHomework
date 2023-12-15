#include "common.h"
#include <vector>
#include"subDetector.h"

extern std::string Delimiter[DELIMITER_NUM]; // 引用在其他地方定义的分隔符数组
extern std::string ReserveWord[RESERVED_WORD_NUM]; // 引用外部定义的保留字数组
extern std::set<std::string> Digit; // 引用在其他地方定义的数字集合
extern std::string Operator[OPERATOR_NUM]; // 引用在其他地方定义的运算符数组
extern std::set<std::string> Identifier; // 引用外部定义的标识符集合



// ==================================================================================================================================================
//                                                    DelimiterDetector
// 在分隔符数组中查找给定的字符串
// ch: 要查找的字符串
int DelimiterDetector::findDelimiter(std::string ch)
{
    // 遍历分隔符数组
    for (int index = 0; index < DELIMITER_NUM; index++) {
        // 如果找到匹配的分隔符，返回其在数组中的索引
        if (ch.find(Delimiter[index]) != -1)
            return index;
    }
    // 如果没有找到匹配的分隔符，返回NOT
    return NOT;
}

// 判断给定字符串的指定位置是否是一个有效的分隔符
// str: 给定的字符串，start: 起始位置的引用，end: 结束位置的引用
// line: 行号的引用，col: 列号的引用
int DelimiterDetector::isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col)
{
    uint currentPosition = start; // 当前处理的位置

    // 调用findDelimiter函数查找当前位置的字符是否是分隔符
    if (findDelimiter(&str[currentPosition]) == NOT) {
        // 如果不是分隔符，返回NOT
        return NOT;
    }
    else {
        // 如果是分隔符，更新start和end位置，并且列号加一
        start = end = currentPosition;
        col++;
        // 返回SUCCESS表示成功找到分隔符
        return SUCCESS;
    }
}
// ==================================================================================================================================================




// ==================================================================================================================================================
//                                                    KeywordDetector
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
    uint currentPosition = start; // 当前处理的位置
    uint currentColumn = col; // 当前列号
    std::string foundWord = ""; // 用于存储识别出的单词

    // 循环遍历字符串中的字符
    while (1) {
        // 如果字符是字母，将其添加到word中
        if (isAlphaOrUnderscore(str[currentPosition])) {
            foundWord += str[currentPosition];
            currentPosition++;
            currentColumn++;
        }
        else {
            break; // 遇到非字母字符时停止循环
        }
    }

    // 在保留字映射表中查找识别出的单词
    auto iter = ReservedWord_Map.find(foundWord);
    if (iter != ReservedWord_Map.end()) { // 如果找到
        end = currentPosition - 1; // 设置结束位置
        col = currentColumn; // 更新列号
        return iter->second; // 返回保留字在映射表中的位置
    }
    else {
        return NOT; // 如果未找到，返回NOT
    }
}
// ==================================================================================================================================================




// ==================================================================================================================================================
//                                                    NumericSequenceValidator
// 构造函数：初始化状态为STATE_START
NumericSequenceValidator::NumericSequenceValidator()
{
    state = this->STATE_START;
}

// 重置状态为STATE_START
void NumericSequenceValidator::resetState()
{
    state = STATE_START;
}

// 检查字符串是否是有效的数字序列
// str: 需要检查的字符串，start: 起始位置的引用，end: 结束位置的引用
// line: 行号的引用，col: 列号的引用
int NumericSequenceValidator::isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col)
{
    // 非初始状态时返回NOT
    if (state != STATE_START)
        return NOT;

    uint currentPosition = start; // 当前检查的位置
    for (;; currentPosition++) {
        // 超出字符串范围时返回NOT
        if (currentPosition > str.size()) {
            return NOT;
        }

        // 根据当前状态和字符进行状态转移
        switch (state) {
        case STATE_START:
            if (isdigit(str[currentPosition])) {
                // 数字字符保持在STATE_START状态
            }
            else if (str[currentPosition] == '.') {
                // 遇到小数点进入STATE_1状态
                state = STATE_1;
            }
            else if (str[currentPosition] == 'e') {
                // 遇到e进入STATE_3状态
                state = STATE_3;
            }
            else {
                // 其他字符进入终止状态
                state = STATE_TERMINATE;
            }
            break;

        case STATE_1:
            if (isdigit(str[currentPosition])) {
                // 数字字符进入STATE_2状态
                state = STATE_2;
            }
            else if (str[currentPosition] == 'e') {
                // 遇到e进入STATE_3状态
                state = STATE_3;
            }
            else if (str[currentPosition] == '.') {
                // 再次遇到小数点进入错误状态
                state = STATE_ERROR;
            }
            else {
                // 其他字符进入终止状态
                state = STATE_TERMINATE;
            }
            break;

        case STATE_2:
            if (isdigit(str[currentPosition])) {
                // 数字字符保持在STATE_2状态
            }
            else if (str[currentPosition] == 'e') {
                // 遇到e进入STATE_3状态
                state = STATE_3;
            }
            else if (str[currentPosition] == '.') {
                // 遇到小数点进入错误状态
                state = STATE_ERROR;
            }
            else {
                // 其他字符进入终止状态
                state = STATE_TERMINATE;
            }
            break;

        case STATE_3:
            if (isdigit(str[currentPosition])) {
                // 数字字符进入STATE_4状态
                state = STATE_4;
            }
            else if (str[currentPosition] == '.' || str[currentPosition] == 'e') {
                // 遇到小数点或e进入错误状态
                state = STATE_ERROR;
            }
            else {
                // 其他字符进入终止状态
                state = STATE_TERMINATE;
            }
            break;

        case STATE_4:
            if (isdigit(str[currentPosition])) {
                // 数字字符保持在STATE_4状态
            }
            else if (str[currentPosition] == '.' || str[currentPosition] == 'e') {
                // 遇到小数点或e进入错误状态
                state = STATE_ERROR;
            }
            else {
                // 其他字符进入终止状态
                state = STATE_TERMINATE;
            }
            break;

        default:
            // 默认情况下进入错误状态
            state = STATE_ERROR;
            break;
        }

        // 如果进入终止状态或错误状态，退出循环
        if (state == STATE_TERMINATE || state == STATE_ERROR)
            break;
    }

    // 如果进入终止状态，处理识别出的数字序列
    if (state == STATE_TERMINATE) {
        end = currentPosition - 1; // 设置结束位置
        state = STATE_START; // 重置状态
        col = col + (currentPosition - start); // 更新列号
        // 将识别出的数字序列加入Digit集合
        std::string temp = str.substr(start, end - start + 1);
        Digit.insert(temp);
        return SUCCESS;
    }
    else {
        state = STATE_START; // 重置状态
        return NOT; // 返回NOT表示未识别出有效数字序列
    }
}
// ==================================================================================================================================================



// ==================================================================================================================================================
//                                                    OperatorDetector
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
    char currentChar = str[start]; // 当前处理的字符

    // 根据字符判断运算符类型
    switch (currentChar) {
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
        end = (str[start + 1] == '=' || (currentChar == str[start + 1] && currentChar != '>' && currentChar != '<')) ? start + 1 : start;
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
// ==================================================================================================================================================



// ==================================================================================================================================================
//                                                    SymbolRecognizer
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
    uint currentPosition = start; // 当前处理的位置
    uint currentColumn = col; // 当前列号
    std::string identifierStr = ""; // 用于存储识别出的标识符

    // 循环遍历字符串中的字符
    while (1) {
        // 如果字符是字母或数字，将其添加到word中
        if (isAlphaOrUnderscore(str[currentPosition]) || isNumericDigit(str[currentPosition])) {
            identifierStr += str[currentPosition];
            currentPosition++;
            currentColumn++;
        }
        else {
            break; // 遇到非字母或数字字符时停止循环
        }
    }

    end = currentPosition - 1; // 设置标识符的结束位置
    col = currentColumn; // 更新列号

    // 如果未识别出标识符，返回NOT
    if (identifierStr == "")
        return NOT;

    // 成功识别标识符，将其添加到标识符集合，并返回SUCCESS
    addToken(identifierStr);
    return SUCCESS;
}
// ==================================================================================================================================================