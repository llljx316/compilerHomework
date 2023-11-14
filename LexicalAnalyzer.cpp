#include <string>
#include "common.h"
#include "util.h"
#include "LexicalAnalyzer.h"
#include <sstream>

// 词法分析器的构造函数，初始化DFA数组
LexicalAnalyzer::LexicalAnalyzer()
{
    DFAlist[RESERVED_WORD] = new KeywordDetector;
    DFAlist[IDENTIFIER] = new SymbolRecognizer;
    DFAlist[OPERATOR] = new OperatorDetector;
    DFAlist[DELIMITER] = new DelimiterDetector;
    DFAlist[DIGIT] = new NumericSequenceValidator;
}

// 词法分析器的析构函数，释放DFA数组内存
LexicalAnalyzer::~LexicalAnalyzer()
{
    for (unsigned int i = 0; i < 5; i++)
    {
        if (DFAlist[i] != nullptr)
        {
            delete DFAlist[i];
            DFAlist[i] = nullptr;
        }
    }
}


// 定义分隔符数组
std::string Delimiter[DELIMITER_NUM] =
{
    "(", ")", "{", "}", "[", "]", ";"
};

// 定义运算符数组
std::string Operator[OPERATOR_NUM] =
{
    // 算术运算符
    "+", "-", "*", "/", "%", "++", "--",
    // 关系运算符
    ">", "<", "==", ">=", "<=", "!=",
    // 逻辑运算符
    "&&", "||", "!",
    // 位运算符
    "&", "|", "~", "^", "<<", ">>",
    // 赋值运算符
    "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", ">>=", "<<=",
    // 三目运算符
    "?", ":",
    // 逗号运算符
    ",",
    // 成员访问运算符
    ".", "->"
};

// 定义保留字数组
std::string ReserveWord[RESERVED_WORD_NUM] =
{
    "int", "double", "char", "float", "break", "continue",
    "while", "if", "else", "for", "return", "function"
};

// 标识符集合
std::set<std::string> Identifier;
// 数字集合
std::set<std::string> Digit;


// 词法分析器的主要分析函数
int LexicalAnalyzer::analyze(std::string& in_str, std::string& out_str, int& pre_line, int& pre_col)
{
    // 预处理输入字符串
    if (preprocessor.preprocess(in_str, out_str, pre_line, pre_col) != 1) {
        // 预处理错误处理
        string out_path = lexicalWrongFileName;
        ofstream outfile(out_path);
        if (!outfile.is_open()) {
            cout << "can't open " << out_path << "\n";
            return -1;
        }
        outfile << pre_line << ' ' << pre_col << ' ' << "Preprocess!";
        return PreProcess_NOT;
    }

    // 初始化分析过程的变量
    int sign = RIGHT_STATUS;
    uint start = 0, end = -1;
    uint line = 1, col = 0;
    stringstream out_stream;
    uint last_line = line, last_col = col;

    // 主循环，遍历输入字符串
    while (1) {
        // 更新开始位置
        start = end + 1;

        // 检查是否已处理完全部字符串
        if (start >= out_str.size()) {
            break;
        }

        // 跳过空白字符，更新行和列的位置
        uint pos = start;
        uint cur_col = col;
        uint cur_line = line;
        while (out_str[pos] == ' ' || out_str[pos] == '\n' || out_str[pos] == '\r') {
            if (out_str[pos] == ' ' || out_str[pos] == '\r') {
                pos++;
                cur_col++;
            }
            else {
                pos++;
                cur_line++;
                cur_col = 0;
            }
            if (pos >= out_str.size()) {
                break;
            }
        }
        start = pos;
        end = pos;
        line = cur_line;
        col = cur_col;

        // 检查是否处理完毕
        if (start >= out_str.size()) {
            sign = RIGHT_STATUS;
            break;
        }

        // 根据当前字符确定进入哪个DFA
        int enter_dfa = -1;
        {
            char ch = out_str[start];
            if (isDelimiterChar(ch)) {
                enter_dfa = DELIMITER;
            }
            else if (isNumericDigit(ch)) {
                enter_dfa = DIGIT;
            }
            else if (isOperatorChar(ch)) {
                enter_dfa = OPERATOR;
            }
        }

        // 如果没有直接确定DFA，遍历其他DFA查找匹配
        if (enter_dfa == -1) {
            int i;
            for (i = 0; i < 5; i++) {
                if (i == OPERATOR || i == DELIMITER || i == DIGIT)
                    continue;
                int result = DFAlist[i]->isAccepted(out_str, start, end, line, col);
                if (result == NOT) {
                    continue;
                }
                else {
                    enter_dfa = i;
                    break;
                }
            }
            if (i >= 5) {
                end = start + 5;
                sign = convertResultToTokenSign(DELIMITER);
            }
        }
        else {
            int result = DFAlist[enter_dfa]->isAccepted(out_str, start, end, line, col);
            if (result == NOT) {
                sign = convertResultToTokenSign(enter_dfa);
            }
        }

        // 如果有错误，终止分析
        if (sign != RIGHT_STATUS) {
            break;
        }
        else {
            // 如果没有错误，记录词法单元
            string value = out_str.substr(start, end - start + 1);
            int sign = convertResultToTokenSign(enter_dfa);
            string type;
            convertTokenSignToOutput(sign, value, type);
            out_stream << line << ' ' << col << ' ' << type << ' ' << value << "\n";
            last_line = line;
            last_col = col;
        }
    }

    // 结果输出到文件
    if (sign == RIGHT_STATUS) {
        // 正确处理的结果
        string out_path = lexicalOutputName;
        ofstream outfile(out_path);
        if (!outfile.is_open()) {
            cout << "can't open " << out_path << "\n";
            return -1;
        }
        outfile << out_stream.str();
        outfile << last_line << ' ' << 1 << ' ' << "#" << ' ' << "#" << "\n";
        outfile.close();
    }
    else {
        // 错误信息输出
        string out_path = lexicalWrongFileName;
        ofstream outfile(out_path);
        if (!outfile.is_open()) {
            cout << "can't open " << out_path << "\n";
            return -1;
        }
        string value = out_str.substr(start, end - start + 1);
        string type;
        convertTokenSignToOutput(sign, value, type);
        outfile << last_line << ' ' << last_col + 1 << ' ' << type << ' ' << value << "\n";
        outfile.close();
    }

    return sign;
}
