#ifndef MIDCODEPARSER_H
#define MIDCODEPARSER_H
#include "lritem.h"
#include "token.h"
#include "parser.h"
#include "Lexeme.h"
#include <tuple>
#include <vector>
#include <string>
#include <set>
#include <map>

typedef std::pair<std::string, int> psi;
typedef std::vector<psi> vpsi;
typedef std::set<LR1> LR1set;

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

class MidCodeParser:public Parser
{
protected:

    std::tuple<Token, std::string> translate(int id, std::string name);

public:
    MidCodeParser() {}


    int analyse(const std::vector<Token>& tokens);
};

#endif // MIDCODEPARSER_H
