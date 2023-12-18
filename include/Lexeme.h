#ifndef LEXEME_H
#define LEXEME_H

#include <string>
#include <set>

class Lexeme
{
public:
    std::string value; // 值
    std::string realV; // 实际值
    int line;         // 行号
    int id;           // 标识符

    // 设置标识符
    void setID(int id)
    {
        this->id = id;
    }

    // 构造函数，允许设置行号、值、实际值和标识符
    Lexeme(int line, std::string value, std::string realV, int id)
        : line(line), value(value), realV(realV), id(id)
    {
    }

    // 拷贝构造函数
    Lexeme(const Lexeme& c)
    {
        this->line = c.line;
        this->value = c.value;
        this->realV = c.realV;
        this->id = c.id;
    }

    Lexeme(){}
};

class NewLexeme:public Lexeme{
public:
    int col;
    int width;
    std::string type;
    std::string rrealv;//for a=1..
    //control&bool properties
    std::set<int> truelist;
    std::set<int> falselist;
    std::set<int> nextlist;
    std::string quad;

    // 构造函数，允许设置行号、值、实际值和标识符
    NewLexeme(int line,int col, std::string value, std::string realV, int id)
        : Lexeme(line,value,realV,id), col(col)
    {
    }

    NewLexeme():Lexeme(){}

    // 拷贝构造函数
    NewLexeme(const NewLexeme& c):Lexeme(c)
    {
        this->width = c.width;
        //        this->line = c.line;
        this->col=c.col;
        //        this->value = c.value;
        //        this->realV = c.realV;
        //        this->id = c.id;
        this->type = c.type;
        this->rrealv = c.rrealv;
        this->falselist = c.falselist;
        this->truelist = c.truelist;
        this->nextlist = c.nextlist;
        this->quad = c.quad;
    }

    NewLexeme(int line,int col,int id)
    {
        this->col=col;
        this->line = line;
        this->id = id;
    }
};


#endif // LEXEME_H
