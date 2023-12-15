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
        string errorPath = "./Wrong_info.txt";
        ofstream errorOutput(errorPath);
        if (!errorOutput.is_open()) {
            cout << "can't open " << errorPath << "\n";
            return -1;
        }
        errorOutput << pre_line << ' ' << pre_col << ' ' << "Preprocess!";
        return PreProcess_NOT;
    }

    // 初始化分析过程的变量
    int status = RIGHT_STATUS;
    uint startIndex = 0, endIndex = -1;
    uint lineNumber = 1, columnNumber = 0;
    stringstream outputStream;
    uint lastLineNumber = lineNumber, lastColumnNumber = columnNumber;

    // 主循环，遍历输入字符串
    while (1) {
        // 更新开始位置
        startIndex = endIndex + 1;

        // 检查是否已处理完全部字符串
        if (startIndex >= out_str.size()) {
            break;
        }

        // 跳过空白字符，更新行和列的位置
        uint position = startIndex;
        uint currentColumn = columnNumber;
        uint currentLine = lineNumber;
        while (out_str[position] == ' ' || out_str[position] == '\n' || out_str[position] == '\r') {
            if (out_str[position] == ' ' || out_str[position] == '\r') {
                position++;
                currentColumn++;
            }
            else {
                position++;
                currentLine++;
                currentColumn = 0;
            }
            if (position >= out_str.size()) {
                break;
            }
        }
        startIndex = position;
        endIndex = position;
        lineNumber = currentLine;
        columnNumber = currentColumn;

        // 检查是否处理完毕
        if (startIndex >= out_str.size()) {
            status = RIGHT_STATUS;
            break;
        }

        // 根据当前字符确定进入哪个DFA
        int dfaIndex = -1;
        {
            char currentChar = out_str[startIndex];
            if (isDelimiterChar(currentChar)) {
                dfaIndex = DELIMITER;
            }
            else if (isNumericDigit(currentChar)) {
                dfaIndex = DIGIT;
            }
            else if (isOperatorChar(currentChar)) {
                dfaIndex = OPERATOR;
            }
        }

        // 如果没有直接确定DFA，遍历其他DFA查找匹配
        if (dfaIndex == -1) {
            int dfaCounter;
            for (dfaCounter = 0; dfaCounter < 5; dfaCounter++) {
                if (dfaCounter == OPERATOR || dfaCounter == DELIMITER || dfaCounter == DIGIT)
                    continue;
                int result = DFAlist[dfaCounter]->isAccepted(out_str, startIndex, endIndex, lineNumber, columnNumber);
                if (result == NOT) {
                    continue;
                }
                else {
                    dfaIndex = dfaCounter;
                    break;
                }
            }
            if (dfaCounter >= 5) {
                endIndex = startIndex + 5;
                status = convertResultToTokenSign(DELIMITER);
            }
        }
        else {
            int result = DFAlist[dfaIndex]->isAccepted(out_str, startIndex, endIndex, lineNumber, columnNumber);
            if (result == NOT) {
                status = convertResultToTokenSign(dfaIndex);
            }
        }

        // 如果有错误，终止分析
        if (status != RIGHT_STATUS) {
            break;
        }
        else {
            // 如果没有错误，记录词法单元
            string tokenValue = out_str.substr(startIndex, endIndex - startIndex + 1);
            int tokenType = convertResultToTokenSign(dfaIndex);
            string tokenTypeStr;
            convertTokenSignToOutput(tokenType, tokenValue, tokenTypeStr);
            outputStream << lineNumber << ' ' << columnNumber << ' ' << tokenTypeStr << ' ' << tokenValue << "\n";
            lastLineNumber = lineNumber;
            lastColumnNumber = columnNumber;
        }
    }

    // 结果输出到文件
    if (status == RIGHT_STATUS) {
        // 正确处理的结果
        string outputPath = "./token_result.txt";
        ofstream resultOutput(outputPath);
        if (!resultOutput.is_open()) {
            cout << "can't open " << outputPath << "\n";
            return -1;
        }
        resultOutput << outputStream.str();
        resultOutput << lastLineNumber << ' ' << 1 << ' ' << "#" << ' ' << "#" << "\n";
        resultOutput.close();
    }
    else {
        // 错误信息输出
        string errorPath = "./Wrong_info.txt";
        ofstream errorOutput(errorPath);
        if (!errorOutput.is_open()) {
            cout << "can't open " << errorPath << "\n";
            return -1;
        }
        string tokenValue = out_str.substr(startIndex, endIndex - startIndex + 1);
        string tokenTypeStr;
        convertTokenSignToOutput(status, tokenValue, tokenTypeStr);
        errorOutput << lastLineNumber << ' ' << lastColumnNumber + 1 << ' ' << tokenTypeStr << ' ' << tokenValue << "\n";
        errorOutput.close();
    }

    return status;
}
