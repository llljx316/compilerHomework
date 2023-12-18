#ifndef LEXEME_H
#define LEXEME_H

#include <string>
#include <set>
#include "token.h"

class oneToken
{
public:
    TokenType type;
    std::string name;
    std::string value;

    int offset;
    oneToken(TokenType type, std::string name, std::string value, int offset)
    {
        this->name = name;
        this->type = type;
        this->offset = offset;
        this->value = value;
    };
};

static int tempid=0;
class sym_tbl
{
    std::string tblName;
    std::string nextTblName;
    sym_tbl* next_table;
    std::vector<Token> syms;
public:
    sym_tbl(std::string tblName )
    {
        this->tblName = tblName;
    }
    void addsys(Token x)
    {
        syms.push_back(x);
    }
    void newTemp(Token& ttoken)
    {
        tempid++;
        syms.push_back(ttoken);

        return;
    }


    Token lookup(std::string name)
    {

        for (auto ite = syms.begin(); ite != syms.end(); ite++) {
            //std::cout << ( * ite).name << std::endl;
            if ((*ite).name == name) {
                return *ite;
            }
        }
        return Token("None",TokenType::OTHER,-1);
    }

};

#endif // LEXEME_H
