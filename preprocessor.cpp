#include "common.h"
#include "preprocessor.h"

// 预处理函数：对输入字符串进行预处理，去除注释等
// in_str: 输入字符串，out_str: 输出字符串，line: 当前行号，col: 当前列号
int Preprocessor::preprocess(std::string& in_str, std::string& out_str, int& line, int& col)
{
    out_str = ""; // 初始化输出字符串
    unsigned int index = 0; // 输入字符串的索引
    int cur_col = col; // 当前列号
    int cur_line = line; // 当前行号

    // 遍历输入字符串
    while (index < in_str.size()) {
        char ch = in_str[index]; // 当前字符

        // 判断是否为注释的开始
        if (ch == '/') {
            char ch1 = in_str[++index]; // 获取下一个字符
            // 处理字符串结束的情况
            if (index >= in_str.size()) {
                line = cur_line;
                col = cur_col;
                return NOT;
            }

            // 处理单行注释
            if (ch1 == '/') {
                while ((ch1 = in_str[++index]) != '\n') {
                    cur_col++;
                }
                out_str += '\n';
                cur_line++;
                cur_col = 0;
            }
            // 处理非注释情况
            else if (ch1 != '*') {
                out_str += '/';
                out_str += ch1;
                cur_col++;
            }
            // 处理多行注释
            else {
                char ch2 = in_str[++index];
                if (index >= in_str.size()) {
                    line = cur_line;
                    col = cur_col;
                    return NOT;
                }
                while (1) {
                    // 跳过注释内容
                    while (ch2 != '*') {
                        if (ch2 == '\n') {
                            cur_line++;
                            cur_col = 0;
                            out_str += '\n';
                        }
                        ch2 = in_str[++index];
                        cur_col++;
                        if (index >= in_str.size()) {
                            line = cur_line;
                            col = cur_col;
                            return NOT;
                        }
                    }
                    ch2 = in_str[++index];
                    cur_col++;
                    if (index >= in_str.size()) {
                        line = cur_line;
                        col = cur_col;
                        return NOT;
                    }
                    // 注释结束
                    if (ch2 == '/') break;
                    else if (ch2 == '\n') {
                        cur_line++;
                        cur_col = 0;
                        out_str += '\n';
                    }
                }
            }
        }
        // 处理制表符
        else if (ch == '\t') {
            out_str += ' ';
            cur_col++;
        }
        // 处理换行符
        else if (ch == '\n') {
            out_str += '\n';
            cur_line++;
            cur_col = 0;
        }
        // 其他字符直接复制
        else {
            out_str += ch;
            cur_col++;
        }
        index++;
    }
    line = cur_line;
    col = cur_col;
    return 1;
}

// 预处理器的接受函数，当前未实现具体功能
// str: 输入字符串，start: 起始位置，end: 结束位置，line: 行号，col: 列号
int Preprocessor::isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col)
{
    return 0;
}
