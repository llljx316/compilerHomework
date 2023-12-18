#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#define _MAX 655        //最大代码长度
#define _MaxCodeLen 655           //最大代码长度
#define _MaxVarNum 256              //变量最大个数
#define _DigitNum 256                //常量最大个数
#define _MaxKW 34               //关键字数量
#define _MaxUptNum 7            //单目运算符最大个数
#define _MaxBptNum 6           //双目运算符最大个数
#define _MaxEndNum 11                //界符最大个数

#include <iostream>

using namespace std;

enum err_type
{
    varOF = 1,
    ptERR = 2,
    conOF = 3
};

typedef struct SymbolTable
{
    int idx;       //标识符所在表的下标
    int type;        //标识符的类型
    int line;        //标识符所在表的行数
    char symbol[20]; //标识符所在表的名称
} Table;

int SymIdx = 0;           //display表的下标
char Symbol[_MaxVarNum][20];  //标识符表
char Digit[_MaxVarNum][20]; //数字表
int SymbolNum = 0;            //变量表的下标
int DigNum = 0;             //常量表的下标
bool errFlag = 0;         //错误标志

const char *const KeyWord[_MaxKW] = {"int", "void", "if", "else", "while", "return",
                                         "signed", "char", "double", "unsigned", "const",
                                         "goto", "for", "float", "break", "class", "case",
                                         "do", "long", "typedef", "static", "friend",
                                         "new", "enum", "try", "short", "continue", "sizeof",
                                         "switch", "private", "catch", "delete", "public", "struct"};
const char U_opt[] = {'+', '-', '*', '/', '=', '<', '>'}; // 单目运算
const char *B_opt[] = {"<=", ">=", ":=", "<>", "==", "!="};                //双目运算符
const char Delimiter[] = {
    '(', ')', ',', ';', '.', '[',
    ']', ':', '{', '}', '"'}; // 界符

/*
0——33：关键字："int", "void", "if", "else", "while", "return"
34：标识符
35：整数
36：小数
37——42：双目运算符："<=", ">=", ":=", "<>", "==", "!="
43——49：单目运算符：'+', '-', '*', '/', '=', '<', '>'
50——60：'(', ')', ',', ';', '.', '[', ']', ':', '{', '}', '"'
*/

class LexAnalyzer
{
public:
    LexAnalyzer();
    void Scanner(char ch[], int ch_len, Table table[_MAX], int line_num);
    void error(char str[20], int line_num, int err_type);
};

#endif // LEXICALANALYZER_H