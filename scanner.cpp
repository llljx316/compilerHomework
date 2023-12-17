#include "scanner.h"
#include "KeyWord.h"
#include "TokenType.h"
#include <fstream>
#include <cctype>
#include<iostream>

using namespace std;

Scanner::Scanner()
{
    proc = Process::BEGIN;
    line = 0;
}

void Scanner::skip_blank()
{
    while (iter != code.cend() && *iter != EOF)
    {
        if (*iter == '\n')
        {
            iter++;
            line++;
        }
        else if (isblank(*iter))
        {
            iter++;
        }
        else
        {
            break;
        }
    }
}

void Scanner::BeginDetector(std::string& name, TokenType& type)
{
    if (std::isdigit(*iter))
        proc = Process::IN_INTEGER;
    else if (std::isalpha(*iter) || *iter == '_')
        proc = Process::IN_ID;
    else if (*iter == ',' || *iter == ';' || *iter == '{' || *iter == '}'
             || *iter == '[' || *iter == ']' || *iter == '(' || *iter == ')'
             || *iter == '#' || *iter == '.' || *iter == '"' || *iter == '\'')
        proc = Process::IN_DELIMITER;
    else
        proc = Process::IN_OPERATOR;
}

void Scanner::KeyWordDetector(std::string& name, TokenType& type)
{
    type = TokenType::KEYWORD;
    proc = Process::END;
}

void Scanner::IDDetector(std::string& name, TokenType& type)
{
    if (std::isalnum(*iter))
    {
        name += *iter;
        iter++;
    }
    else if (KWset.find(name) != KWset.end())
    {
        proc = Process::IN_KEYWORD;
    }
    else
    {
        type = TokenType::ID;
        proc = Process::END;
    }
}

void Scanner::IntDetector(std::string& name, TokenType& type)
{
    if (std::isdigit(*iter))
    {
        name += *iter;
        iter++;
    }
    else if (*iter == '.')
    {
        name += *iter;
        iter++;
        proc = Process::IN_FLOAT;
    }
    else
    {
        type = TokenType::INT;
        proc = Process::END;
    }
}

void Scanner::FloatDetector(std::string& name, TokenType& type)
{
    if (std::isdigit(*iter))
    {
        name += *iter;
        iter++;
    }
    else
    {
        type = TokenType::FLOAT;
        proc = Process::END;
    }
}

void Scanner::OperatorDetector(std::string& name, TokenType& type)
{
    if (*iter == '*' || *iter == '/' || *iter == '%' || *iter == '^' || *iter == '~')
    {
        name += *iter;
        iter++;
        type = TokenType::OPERATOR;
        proc = Process::END;
    }
    else if (*iter == '!' || *iter == '=' || *iter == '&' || *iter == '|' || *iter == '<' || *iter == '>' || *iter == '+' || *iter == '-')
    {
        name += *iter;
        const char lastOp = *iter;
        iter++;
        if (iter != code.cend())
        {
            if (lastOp == '!' && *iter == '=')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '=' && *iter == '=')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '&' && *iter == '&')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '|' && *iter == '|')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '<' && *iter == '=')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '>' && *iter == '=')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '+' && *iter == '+')
            {
                name += *iter;
                *iter++;
            }
            else if (lastOp == '-' && *iter == '-')
            {
                name += *iter;
                *iter++;
            }
            type = TokenType::OPERATOR;
            proc = Process::END;
        }
    }
    else
    {
        type = TokenType::OTHER;
        proc = Process::END;
    }
}

void Scanner::DelimiterDetector(std::string& name, TokenType& type)
{
    name += *iter;
    iter++;
    type = TokenType::DELIMITER;
    proc = Process::END;
}

Token Scanner::EndDetector(const std::string& name, const TokenType& type)
{
    return Token(name, type, line);
}

void Scanner::CharDetector(std::string& name, TokenType& type)
{
    if (*iter == '\'')
    {
        type = TokenType::CHAR;
        proc = Process::END;
    }
    name += *iter;
    iter++;
}

/********************编译预处理，取出无用的字符和注释**********************/
std::string Scanner::Preprocessing(std::string input_str, int Input_str_len)
{
    //char tmp_str[_MaxCodeLen];
    std::string tmp_str;
    for (int i = 0; i <= Input_str_len; i++)
    {
        if (input_str[i] == '/' && input_str[i + 1] == '/')
        {
            //若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
            while (input_str[i] != '\n')
            {
                i++;//向后扫描
            }
        }
        if (input_str[i] == '/' && input_str[i + 1] == '*')
        {
            //若为多行注释“/* 。。。*/”则去除该内容
            i += 2;
            while (input_str[i] != '*' || input_str[i + 1] != '/')
            {
                i++;//继续扫描
                if (input_str[i] == '$')
                {
                    printf("注释出错，没有找到 */，程序结束！！！\n");
                    exit(0);
                }
            }
            i += 2;//跨过“*/”
        }


        if (input_str[i] != '\v' && input_str[i] != '\r')
        {
            //若出现无用字符，则过滤；否则加载
            //tmp_str[count++] = input_str[i];
            tmp_str += input_str[i];
        }
    }
    //tmp_str[count] = '\0';
    input_str = tmp_str;
    return tmp_str;
    //strcpy(input_str, tmp_str);//产生净化之后的源程序
}

bool Scanner::open_file(const std::string& file_name)
{
    std::ifstream in(file_name);
    proc = Process::BEGIN;
    this->file_name = file_name;
    code = "";
    line = 1;


    if (in)
    {
        code = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        iter = code.cbegin();

        return true;
    }
    else
    {
        std::cerr << file_name << "开启失败" << std::endl;
        return false;
    }
}

Token Scanner::get_next_token()
{
    proc = Process::BEGIN;
    std::string name;
    TokenType type = TokenType::OTHER;
    skip_blank();
    while ((iter != code.cend() && *iter != EOF) || proc == Process::END)
    {
        switch (proc)
        {
        case Process::BEGIN:
            BeginDetector(name, type);
            break;
        case Process::IN_DELIMITER:
            DelimiterDetector(name, type);
            break;
        case Process::IN_FLOAT:
            FloatDetector(name, type);
            break;
        case Process::IN_ID:
            IDDetector(name, type);
            break;
        case Process::IN_INTEGER:
            IntDetector(name, type);
            break;
        case Process::IN_KEYWORD:
            KeyWordDetector(name, type);
            break;
        case Process::IN_OPERATOR:
            OperatorDetector(name, type);
            break;
        case Process::IN_CHAR:
            CharDetector(name, type);
            break;
        case Process::END:
            auto token = EndDetector(name, type);
            return token;

        }
    }
    return Token("", TokenType::OTHER, line);
}

std::vector<Token> Scanner::getTokens(const std::string file_name)
{
    std::vector<Token> tokens;
    if (open_file(file_name))
    {
        for (Token token = get_next_token(); token; token = get_next_token())
        {
            tokens.push_back(token);
        }
    }
    tokens.push_back(Token("$", TokenType::DELIMITER, line));
    return tokens;
}
