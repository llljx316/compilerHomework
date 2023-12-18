#include "lexicalanalyzer.h"
#include <cstring>

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

LexicalAnalyzer::LexicalAnalyzer()
{

}

void LexicalAnalyzer::error(char temp_str[20], int line_num, int err_type)
{
    cout << " \nError :    ";
    switch (err_type)
    {
    case varOF:
        cout << "第" << line_num - 1 << "行" << temp_str << " 变量的长度超过限制！\n";
        errFlag = 1;
        break;
    case ptERR:
        cout << "第" << line_num - 1 << "行" << temp_str << " 小数点错误！\n";
        errFlag = 1;
        break;
    case conOF:
        cout << "第" << line_num - 1 << "行" << temp_str << " 常量的长度超过限制！\n";
        errFlag = 1;
        break;
    }
}

void LexicalAnalyzer::Scanner(char ch[], int ch_len, Table table[_MAX], int line_num)
{
    int ch_idx = 0;

    while (ch_idx < ch_len) //对输入的字符扫描
    {
        /*处理空格和tab*/
        while (ch[ch_idx] == ' ' || ch[ch_idx] == 9) //忽略空格和tab
        {
            ch_idx++;
        }
        /*处理换行符*/
        while (ch[ch_idx] == 10) //遇到换行符，行数加1
        {
            line_num++;
            ch_idx++;
        }

        /*标识符*/
        if (isalpha(ch[ch_idx])) //以字母、下划线开头
        {
            char temp_str[256];
            int temp_str_len = 0;
            while (isalpha(ch[ch_idx]) || ch[ch_idx] == '_') //是字母、下划线
            {
                temp_str[temp_str_len++] = ch[ch_idx];
                ch_idx++;
                while (isdigit(ch[ch_idx])) //不是第一位，可以为数字
                {
                    temp_str[temp_str_len++] = ch[ch_idx];
                    ch_idx++;
                }
            }
            temp_str[temp_str_len] = 0;      //字符串结束符
            if (strlen(temp_str) > 20) //标识符超过规定长度，报错处理
            {
                error(temp_str, line_num, 1);
            }
            else
            {
                int i;
                for (i = 0; i < _MaxKW; i++)      //与关键字匹配
                    if (strcmp(temp_str, KeyWord[i]) == 0) //是关键字，写入table表中
                    {
                        strcpy(table[SymIdx].symbol, temp_str);
                        table[SymIdx].type = i; //关键字
                        table[SymIdx].line = line_num;
                        table[SymIdx].idx = i;
                        SymIdx++;
                        break;
                    }
                if (i >= _MaxKW) //不是关键字
                {

                    table[SymIdx].idx = SymbolNum;
                    strcpy(Symbol[SymbolNum++], temp_str);
                    table[SymIdx].type = _MaxKW; //变量标识符
                    strcpy(table[SymIdx].symbol, temp_str);
                    table[SymIdx].line = line_num;
                    SymIdx++;
                }
            }
        }

        /*常数*/
        // else if(isdigit(ch[ch_idx])&&ch[ch_idx]!='0') //遇到数字
        else if (isdigit(ch[ch_idx])) //遇到数字
        {
            int flag = 0;
            char temp_str[256];
            int temp_str_len = 0;
            while (isdigit(ch[ch_idx]) || ch[ch_idx] == '.') //数字和小数点
            {
                if (ch[ch_idx] == '.') // flag表记小数点的个数，0时为整数，1时为小数，2时出错
                    flag++;
                temp_str[temp_str_len++] = ch[ch_idx];
                ch_idx++;
            }
            temp_str[temp_str_len] = 0;
            if (strlen(temp_str) > 20) //常量标识符超过规定长度20，报错处理
            {
                error(temp_str, line_num, 3);
            }
            if (flag == 0)
            {
                table[SymIdx].type = _MaxKW + 1; //整数
            }
            if (flag == 1)
            {
                table[SymIdx].type = _MaxKW + 2; //小数
            }
            if (flag > 1)
            {
                error(temp_str, line_num, 2);
            }
            table[SymIdx].idx = DigNum;
            strcpy(Digit[DigNum++], temp_str);

            strcpy(table[SymIdx].symbol, temp_str);
            table[SymIdx].line = line_num;
            SymIdx++;
        }

        /*运算符*/
        else
        {
            int errFlag; //用来区分是不是无法识别的标识符，0为运算符，1为界符

            char temp_str[3];
            temp_str[0] = ch[ch_idx];
            temp_str[1] = ch[ch_idx + 1];
            temp_str[2] = '\0';
            int i;
            for (i = 0; i < _MaxBptNum; i++) // MaxOptBNum)
                if (strcmp(temp_str, B_opt[i]) == 0)
                {
                    errFlag = 0;
                    table[SymIdx].type = _MaxKW + 3 + i;
                    strcpy(table[SymIdx].symbol, temp_str);
                    table[SymIdx].line = line_num;
                    table[SymIdx].idx = i;
                    SymIdx++;
                    ch_idx = ch_idx + 2;
                    break;
                }
            if (i >= _MaxBptNum)
            {
                for (int k = 0; k < _MaxUptNum; k++)
                    if (U_opt[k] == ch[ch_idx])
                    {
                        errFlag = 0;
                        table[SymIdx].type = _MaxKW + 3 + _MaxBptNum + k;
                        table[SymIdx].symbol[0] = ch[ch_idx];
                        table[SymIdx].symbol[1] = 0;
                        table[SymIdx].line = line_num;
                        table[SymIdx].idx = k;
                        SymIdx++;
                        ch_idx++;
                        break;
                    }

                /*界符*/
                for (int j = 0; j < _MaxEndNum; j++)
                    if (Delimiter[j] == ch[ch_idx])
                    {
                        errFlag = 1;
                        table[SymIdx].line = line_num;
                        table[SymIdx].symbol[0] = ch[ch_idx];
                        table[SymIdx].symbol[1] = 0;
                        table[SymIdx].idx = j;
                        table[SymIdx].type = _MaxKW + 3 + _MaxBptNum + _MaxUptNum + j;
                        SymIdx++;
                        ch_idx++;
                    }
                /*其他无法识别字符*/
                if (errFlag != 0 && errFlag != 1) //开头的不是字母、数字、运算符、界符
                {
                    char temp_str[256];
                    int temp_str_len = -1;
                    temp_str[temp_str_len++] = ch[ch_idx];
                    ch_idx++;

                    while (*ch != ' ' || *ch != 9 || ch[ch_idx] != 10) //
                    {
                        temp_str[temp_str_len++] = ch[ch_idx];
                        ch_idx++;
                    }
                    temp_str[temp_str_len] = 0;
                    table[SymIdx].type = 1100000;
                    strcpy(table[SymIdx].symbol, temp_str);
                    table[SymIdx].line = line_num;
                    table[SymIdx].idx = -2;
                    SymIdx++;
                }
            }
        }
    }
}
