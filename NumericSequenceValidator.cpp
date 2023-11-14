#include "NumericSequenceValidator.h"
#include "LexicalAnalyzer.h"

extern std::set<std::string> Digit; // 引用在其他地方定义的数字集合

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

    uint i = start; // 当前检查的位置
    for (;; i++) {
        // 超出字符串范围时返回NOT
        if (i > str.size()) {
            return NOT;
        }

        // 根据当前状态和字符进行状态转移
        switch (state) {
        case STATE_START:
            if (isdigit(str[i])) {
                // 数字字符保持在STATE_START状态
            }
            else if (str[i] == '.') {
                // 遇到小数点进入STATE_1状态
                state = STATE_1;
            }
            else if (str[i] == 'e') {
                // 遇到e进入STATE_3状态
                state = STATE_3;
            }
            else {
                // 其他字符进入终止状态
                state = STATE_TERMINATE;
            }
            break;

        case STATE_1:
            if (isdigit(str[i])) {
                // 数字字符进入STATE_2状态
                state = STATE_2;
            }
            else if (str[i] == 'e') {
                // 遇到e进入STATE_3状态
                state = STATE_3;
            }
            else if (str[i] == '.') {
                // 再次遇到小数点进入错误状态
                state = STATE_ERROR;
            }
            else {
                // 其他字符进入终止状态
                state = STATE_TERMINATE;
            }
            break;

        case STATE_2:
            if (isdigit(str[i])) {
                // 数字字符保持在STATE_2状态
            }
            else if (str[i] == 'e') {
                // 遇到e进入STATE_3状态
                state = STATE_3;
            }
            else if (str[i] == '.') {
                // 遇到小数点进入错误状态
                state = STATE_ERROR;
            }
            else {
                // 其他字符进入终止状态
                state = STATE_TERMINATE;
            }
            break;

        case STATE_3:
            if (isdigit(str[i])) {
                // 数字字符进入STATE_4状态
                state = STATE_4;
            }
            else if (str[i] == '.' || str[i] == 'e') {
                // 遇到小数点或e进入错误状态
                state = STATE_ERROR;
            }
            else {
                // 其他字符进入终止状态
                state = STATE_TERMINATE;
            }
            break;

        case STATE_4:
            if (isdigit(str[i])) {
                // 数字字符保持在STATE_4状态
            }
            else if (str[i] == '.' || str[i] == 'e') {
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
        end = i - 1; // 设置结束位置
        state = STATE_START; // 重置状态
        col = col + (i - start); // 更新列号
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