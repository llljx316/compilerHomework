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
//    std::vector<Production> grammar;   // store grammar
//    std::vector<LR1set > closure_list;  // closure list
//    std::map<LR1set, int> closure_map;  // closure map
//    std::vector<vpsi > transfer;       // transfer function
//    std::vector<std::map<std::string, psi>> action;     // action table
//    std::vector<std::map<std::string, int>> go;         // goto table
//    std::set<std::string > var_set;                 // variable symbol set


//    std::set<std::string> get_first(const std::vector<std::string>& beta);   // get first(beta), beta = A1A2...An
//    std::map<std::string, std::set<std::string>> first_map;  // first_map[X] = first(X), X is a symbol
//    void gen_first_map();                                   // build first map

//    std::set<LR1> get_closure(const LR1& item);      // get closure
//    void get_closure(LR1set& closure);                       // merge closure
    std::tuple<Token, std::string> translate(int id, std::string name);

//    std::tuple<NewLexeme, std::string> senaticCheck(int id, std::string name);
public:
    MidCodeParser() {}

//    bool open_file(const std::string& fileName);
//    void build();

//    std::vector<Production> read_grammer();
//    std::set<std::string> get_first(const std::string& symbol);                   // get first(symbol)

//    std::vector< LR1set > get_closure();
//    std::map< LR1set, int> get_closure_map();
//    std::vector< vpsi > get_transfer();
//    std::vector< std::map<std::string, psi>> get_action();
//    std::vector< std::map<std::string, int>> get_go();
    int analyse(const std::vector<Token>& tokens);
//    std::set< std::string > get_var();
//    std::set< std::string > get_termi();
//    std::set<std::string > termi_set;                 // terminal symbol set

};

#endif // MIDCODEPARSER_H
