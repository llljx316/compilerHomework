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


extern int SymIdx;
extern char Symbol[_MaxVarNum][20];
extern char Digit[_MaxVarNum][20];
extern int SymbolNum;
extern int DigNum;
extern bool errFlag;

extern const char *const KeyWord[_MaxKW];
extern const char U_opt[];
extern const char *B_opt[];
extern const char Delimiter[];

/*
0——33：关键字："int", "void", "if", "else", "while", "return"
34：标识符
35：整数
36：小数
37——42：双目运算符："<=", ">=", ":=", "<>", "==", "!="
43——49：单目运算符：'+', '-', '*', '/', '=', '<', '>'
50——60：'(', ')', ',', ';', '.', '[', ']', ':', '{', '}', '"'
*/

class LexicalAnalyzer
{
public:
    LexicalAnalyzer();
    void Scanner(char ch[], int ch_len, Table table[_MAX], int line_num);
    void error(char str[20], int line_num, int err_type);
};

#endif // LEXICALANALYZER_H
