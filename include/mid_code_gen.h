#ifndef MID_CODE_GEN_H
#define MID_CODE_GEN_H
#include <string>
#include "parser.h"



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

struct Quaternion
{
    std::string type;
    std::string a;
    std::string b;
    // int addr=INIT_STATE;//init_state
    std::string addr = "INIT_STATE";//init_state
    Quaternion(std::string type, std::string a, std::string b, std::string addr) :type(type), a(a), b(b), addr(addr) {};
    Quaternion() { addr = "INIT_STATE"; };
};

class oneLexeme
{
public:
    std::string type;
    std::string name;
    std::string value;

    int offset;
    oneLexeme(std::string type, std::string name, std::string value, int offset)
    {
        this->name = name;
        this->type = type;
        this->offset = offset;
        this->value = value;
    }
};


static int tempid=0;
class sym_tbl
{
    std::string tblName;
    std::string nextTblName;
    sym_tbl* next_table;
    std::vector<oneLexeme> syms;
public:
    sym_tbl(std::string tblName )
    {
        this->tblName = tblName;
    }
    void addsys(oneLexeme x)
    {
        syms.push_back(x);
    }
    std::string newTemp(std::string type, std::string value)
    {
        tempid++;
        std::string x = "T";
        oneLexeme tmp(type,x + std::to_string(tempid),value,-1);
        syms.push_back(tmp);

        return x + std::to_string(tempid);
    }


    oneLexeme lookup(std::string name)
    {

        for (auto ite = syms.begin(); ite != syms.end(); ite++) {
            //std::cout << ( * ite).name << std::endl;
            if ((*ite).name == name) {
                return *ite;
            }
        }
        return oneLexeme("None","None","None",-1);
    }

};


class mid_code_gen_parser: public Parser
{
public:
    std::tuple<bool,std::string,int,int> check(const std::string path);
    std::stack<NewLexeme> symbolS;
    mid_code_gen_parser();


protected:
    //semantic actions

//    static void makelist(int quad, std::set<int>* dst);
//    static void emit(std::string type, std::string a, std::string b, std::string addr);
//    static void merge(std::set<int>* list1, std::set<int>* list2, std::set<int>* dst);
//    static int backpatch(std::set<int>list, std::string addr);
//    std::tuple<NewLexeme, std::string> Default_expression();

};


#endif // MID_CODE_GEN_H


