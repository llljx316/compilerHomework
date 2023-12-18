#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <set>
#include "TokenType.h"

class Token
{
public:
    Token()
    {
        name = "";
        type = TokenType::OTHER;
        line = 0;
    }

    Token(std::string name, TokenType type, size_t line)
    {
        this->name = name;
        this->type = type;
        this->line = line;
    }

    operator bool()
    {
        if (type == TokenType::OTHER)
            return false;
        else
            return true;
    }

    void set_name(const std::string& name);
    std::string get_name() const;
    void set_type(TokenType type);
    TokenType get_type() const;
    std::string get_type_output() const;
    void set_line(size_t line);
    size_t get_line() const;


    std::string name;
    TokenType type;
    size_t line;

    std::set<int> truelist;
    std::set<int> falselist;
    std::set<int> nextlist;
    int quad;
    std::string realV;//实际值

private:


};

#endif // TOKEN_H
