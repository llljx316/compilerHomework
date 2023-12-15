#include "mid_code_gen.h"
#include <fstream>
#include <string>;

using namespace std;

#define CLOSE_FILES f.close();of.close();f1.close();
#define SUCCESS 0
#define ERROR -1

//全局变量 语义分析
std::stack<NewLexeme> LexemeS;
std::stack<sym_tbl> tableS;//symbol table
int nextquad = 0;
std::vector<Quaternion> QuadrupleForm;//TODO:或者直接push 就以全局的nextquad为下标进行存储
std::tuple<NewLexeme, std::string>(*p[229])();//语义分析指针
int offset;
std::stack<int> offsetS;

bool _IsDigit(const string a)
{
    for (auto it : a)
        if (!isdigit(it))
            return false;
    return true;
}

void makelist(int quad, std::set<int>* dst)
{
    dst->clear();
    dst->insert(quad);
}

void emit(string type, string a, string b, string addr)
{
    // int i=nextquad;
    // QuadrupleForm[i]=Quaternion(type,a,b,addr);
    QuadrupleForm.push_back(Quaternion(type, a, b, addr));
    nextquad++;
    // f1 << "( " << type << ", " << a << " " << b << " " << addr << " )"<<std::endl ;
}


int backpatch(std::set<int>list, string addr)
{
    for (int i : list) {
        if (QuadrupleForm[i].addr != "INIT_STATE")
        {
            string temp = QuadrupleForm[i].addr;
            if (_IsDigit(temp))
                return ERROR;
        }//already exist
        else
            QuadrupleForm[i].addr = addr;
    }
    return SUCCESS;
}
void merge(std::set<int>* list1, std::set<int>* list2, std::set<int>* dst)
{
    std::set_union(list1->begin(), list1->end(), list2->begin(), list2->end(), std::inserter(*dst, dst->begin()));
}


//default function to get son's attribute
std::tuple<NewLexeme, std::string> Default_expression()
{
    NewLexeme id = LexemeS.top();
    LexemeS.pop();
    NewLexeme temp(LINEOFNONT,LINEOFNONT, id.value,id.realV,0);
    NewLexeme* E = &temp;
    E->nextlist = id.nextlist;
    E->falselist = id.falselist;
    E->truelist = id.truelist;
    E->quad = id.quad;

    return std::make_tuple(*E, "None");
}
//bool expression
//-----------
//bool operator
//58: logical_or_expression-->logical_or_expression OR_OP logical_and_expression
std::tuple<NewLexeme, std::string> OR_expression()//E->E1 or E2
{
    NewLexeme E1 = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();//OR
    NewLexeme E2 = LexemeS.top();
    LexemeS.pop();
    NewLexeme temp(LINEOFNONT,LINEOFNONT, 0);
    NewLexeme* E = &temp;

    E->quad = std::to_string(nextquad);//replace the function of M

    if (backpatch(E1.falselist, E2.quad) == ERROR)
        return std::make_tuple(*E, "SEMANTIC ERROR::backpatch error in OR_expression in " + E1.realV + " falselist\n");
    // return std::make_tuple(*E, "SEMANTIC ERROR::backpatch error in " + E1.value + " falselist\n");

    merge(&(E1.truelist), &(E2.truelist), &(E->truelist));
    E->falselist = E2.falselist;

    return std::make_tuple(*E, "None");
}
//56: logical_and_expression-->logical_and_expression AND_OP inclusive_or_expression
std::tuple<NewLexeme, std::string> AND_expression()//E->E1 and E2
{
    NewLexeme E1 = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();//AND
    NewLexeme E2 = LexemeS.top();
    LexemeS.pop();
    NewLexeme temp(LINEOFNONT,LINEOFNONT, 0);
    NewLexeme* E = &temp;

    E->quad = std::to_string(nextquad);//replace the function of M

    if (backpatch(E1.truelist, E2.quad) == ERROR)
        return std::make_tuple(*E, "SEMANTIC ERROR::backpatch error in AND_expression in " + E1.realV + " truelist\n");
    // return std::make_tuple(*E, "SEMANTIC ERROR::backpatch error in " + E1.value + " truelist\n");
    E->truelist = E2.truelist;
    merge(&(E1.falselist), &(E2.falselist), &(E->falselist));

    return std::make_tuple(*E, "None");
}
//20: unary_expression--> unary_operator cast_expression
std::tuple<NewLexeme, std::string> NOT_expression()//E->not E1
{
    LexemeS.pop();//NOT
    NewLexeme E1 = LexemeS.top();
    LexemeS.pop();

    NewLexeme temp(LINEOFNONT,LINEOFNONT, 0);
    NewLexeme* E = &temp;

    E->quad = std::to_string(nextquad);//replace the function of M

    E->truelist = E1.falselist;
    E->falselist = E1.truelist;

    return std::make_tuple(*E, "None");
}
//1: primary_expression--> IDENTIFIER——may not，or will cause confliction
//4: primary_expression  --> ( expression ) ——may yes
std::tuple<NewLexeme, std::string> Identifier_expression()//E->id
{
    LexemeS.pop();// (
    NewLexeme id = LexemeS.top();
    LexemeS.pop();// expression
    LexemeS.pop();// )

    NewLexeme temp(LINEOFNONT,LINEOFNONT,0);
    NewLexeme* E = &temp;

    E->quad = std::to_string(nextquad);//replace the function of M

    makelist(nextquad, &E->truelist);
    makelist(nextquad + 1, &E->falselist);
    emit("jnz", id.value, "None", "INIT_STATE");
    emit("j", "None", "None", "INIT_STATE");

    return std::make_tuple(*E, "None");
}
//根据语法文件：bool里的（）被包含在控制语句里了
// int BRANKET_expression(symbol*E,symbol*E1)
// {
//     E->quad=nextquad;//replace the function of M
//     E->truelist=E1->truelist;
//     E->falselist=E1->falselist;

//     return SUCCESS_;
// }
// 42: relational_expression  --> relational_expression < shift_expression
// 43: relational_expression  --> relational_expression > shift_expression
// 44: relational_expression  --> relational_expression LE_OP shift_expression
// 45: relational_expression  --> relational_expression GE_OP shift_expression
// 47: equality_expression  --> equality_expression EQ_OP relational_expression
// 48: equality_expression  --> equality_expression NE_OP relational_expression
std::tuple<NewLexeme, std::string> Relop_expression()//E->id1 relop id2
{
    NewLexeme id1 =  LexemeS.top();
    LexemeS.pop();
    NewLexeme relop =  LexemeS.top();
    LexemeS.pop();
    NewLexeme id2 =  LexemeS.top();
    LexemeS.pop();

    NewLexeme temp(LINEOFNONT,LINEOFNONT,0);
    NewLexeme* E = &temp;
    //revise
    string relop_op = relop.realV;
    if (relop.realV == "LE_OP")
        relop_op = "<=";
    else if (relop.realV == "GE_OP")
        relop_op = ">=";
    else if (relop.realV == "EQ_OP")
        relop_op = "=";
    else if (relop.realV == "NE_OP")
        relop_op = "!=";

    E->quad = std::to_string(nextquad);//replace the function of M

    makelist(nextquad, &E->truelist);
    makelist(nextquad + 1, &E->falselist);

    string type = "j" + relop_op;
    emit(type, id1.value, id2.value, "INIT_STATE");
    emit("j", "None", "None", "INIT_STATE");

    return std::make_tuple(*E, "None");
}
//control sentences
//-----------
//207: selection_statement--> IF ( expression ) statement
std::tuple<NewLexeme, std::string> if_()//S->if E S1
{
    LexemeS.pop();//IF
    LexemeS.pop();//(
    NewLexeme  E =  LexemeS.top();
    LexemeS.pop();//expression
    LexemeS.pop();//)
    NewLexeme  S1 =  LexemeS.top();
    LexemeS.pop(); //statement

    NewLexeme temp(LINEOFNONT, LINEOFNONT, 0);
    NewLexeme* S = &temp;

    S->quad = std::to_string(nextquad);//replace the function of M

    if (backpatch(E.truelist, S1.quad) == ERROR)//S1->quad has been stored in relop/bool operation
        return std::make_tuple(*S, "SEMANTIC ERROR::backpatch error in if in " + E.realV + " truelist\n");
    // return std::make_tuple(*S, "SEMANTIC ERROR::backpatch error in " + E.value + " truelist\n");
    merge(&E.falselist, &S1.nextlist, &S->nextlist);

    return std::make_tuple(*S, "None");
}
//208: selection_statement--> IF ( expression ) statement ELSE statement
std::tuple<NewLexeme, std::string> if_else()//S->if E S1 else S2
{
    LexemeS.pop();//IF
    LexemeS.pop();//(
    NewLexeme E =  LexemeS.top();
    LexemeS.pop();//expression
    LexemeS.pop();//)
    NewLexeme S1 =  LexemeS.top();
    LexemeS.pop(); //statement
    NewLexeme N =  LexemeS.top();
    LexemeS.pop(); // ELSE(replace N)
    NewLexeme S2 =  LexemeS.top();
    LexemeS.pop(); //statement

    NewLexeme temp(LINEOFNONT,LINEOFNONT,  0);
    NewLexeme* S = &temp;

    S->quad = std::to_string(nextquad);//replace the function of M

    if (backpatch(E.truelist, S1.quad) == ERROR)
        return std::make_tuple(*S, "SEMANTIC ERROR::backpatch error in if_else in " + E.realV + " truelist\n");
    // return std::make_tuple(*S, "SEMANTIC ERROR::backpatch error in " + E.value + " truelist\n");
    if (backpatch(E.falselist, S2.quad) == ERROR)
        return std::make_tuple(*S, "SEMANTIC ERROR::backpatch error in if_else in " + E.realV + " falselist\n");
    // return std::make_tuple(*S, "SEMANTIC ERROR::backpatch error in " + E.value + " falselist\n");
    merge(&S1.nextlist, &S2.nextlist, &S->nextlist);
    merge(&S->nextlist, &N.nextlist, &S->nextlist);

    return std::make_tuple(*S, "None");
}
//210: iteration_statement--> WHILE ( expression ) statement
std::tuple<NewLexeme, std::string> while_()//S->while E do S1
{
    LexemeS.pop();//WHILE
    LexemeS.pop();//(
    NewLexeme E =  LexemeS.top();
    LexemeS.pop();//expression
    LexemeS.pop();//)
    NewLexeme S1 =  LexemeS.top();
    LexemeS.pop(); //statement

    NewLexeme temp(LINEOFNONT,LINEOFNONT,  0);
    NewLexeme* S = &temp;

    S->quad = std::to_string(nextquad);//replace the function of M

    if (backpatch(S1.nextlist, E.quad) == ERROR)
        return std::make_tuple(*S, "SEMANTIC ERROR::backpatch error in while in " + S1.realV + " nextlist\n");
    if (backpatch(E.truelist, S1.quad) == ERROR)
        return std::make_tuple(*S, "SEMANTIC ERROR::backpatch error in while in " + E.realV + " truelist\n");
    S->nextlist = E.falselist;
    emit("j", "None", "None", E.quad);

    return std::make_tuple(*S, "None");
}
std::tuple<NewLexeme, std::string> end_sentence()//L->L1;
{
    NewLexeme L1 =  LexemeS.top();
    LexemeS.pop();//expression
    LexemeS.pop();// ;

    NewLexeme temp(LINEOFNONT,LINEOFNONT,  0);
    NewLexeme* L = &temp;


    if (!L1.nextlist.empty()) {//really bool/control sentences
        if (backpatch(L1.nextlist, std::to_string(nextquad)) == ERROR)
            return std::make_tuple(*L, "SEMANTIC ERROR::backpatch error in end_sentence in " + L1.realV + " nextlist\n");
        // return std::make_tuple(*L, "SEMANTIC ERROR::backpatch error in " + L1.value + " nextlist\n");
        L->nextlist = L1.nextlist;
    }
    return std::make_tuple(*L, "None");
}
//205 : expression_statement -> ;
std::tuple<NewLexeme, std::string> end_sentence_2()//L-> ;
{
    NewLexeme delimiter = LexemeS.top();
    LexemeS.pop();// ;

    NewLexeme temp(LINEOFNONT,LINEOFNONT,  0);
    NewLexeme* L = &temp;
    L->quad = std::to_string(nextquad);
    makelist(nextquad, &L->nextlist);

    return std::make_tuple(*L, "None");
}

std::tuple<NewLexeme, std::string>p2()
{
    NewLexeme const_exp = LexemeS.top();
    LexemeS.pop();

    NewLexeme pushWord(LINEOFNONT,LINEOFNONT, "primary_expression", const_exp.realV, 0);
    pushWord.type = "CONSTANT";
    return std::make_tuple(pushWord, "None");
}

std::tuple<NewLexeme, std::string>p11()//postfix_expression  --> postfix_expression INC_OP
{
    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();

    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;

    sym_tbl cur_t = tableS.top();
    oneLexeme left = cur_t.lookup(a);

    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str()) + 1));

    emit("+", a, "1", temp);
    emit(":=", temp, "None", a);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "postfix_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}
std::tuple<NewLexeme, std::string>p12()//postfix_expression  --> postfix_expression INC_OP
{
    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();

    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;

    sym_tbl cur_t = tableS.top();
    oneLexeme left = cur_t.lookup(a);

    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str()) - 1));

    emit("-", a, "1", temp);
    emit(":=", temp, "None", a);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "postfix_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}
//additive_expression-- > additive_expression + multiplicative_expression
std::tuple<NewLexeme, std::string>p36()
{
    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    NewLexeme cast_exp = LexemeS.top();
    LexemeS.pop();
    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;
    std::string b = cast_exp.realV;
    sym_tbl cur_t = tableS.top();
    tableS.pop();
    oneLexeme left = cur_t.lookup(a);
    oneLexeme right = cur_t.lookup(b);
    if (left.name == "None" && mul_exp.type != "CONSTANT" || right.name == "None" && cast_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }
    /*if (left.type != right.type) {
        wrongInfo = "wrong type";
        NewLexeme re(LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }*/
    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str()) + atoi(right.value.c_str())));
    tableS.push(cur_t);
    emit("+", a, b, temp);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "additive_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}
std::tuple<NewLexeme, std::string>p32()
{
    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    NewLexeme cast_exp = LexemeS.top();
    LexemeS.pop();
    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;
    std::string b = cast_exp.realV;
    sym_tbl cur_t = tableS.top();
    tableS.pop();
    oneLexeme left = cur_t.lookup(a);
    oneLexeme right = cur_t.lookup(b);
    if (left.name == "None" && mul_exp.type != "CONSTANT" || right.name == "None" && cast_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }
    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str()) * atoi(right.value.c_str())));
    tableS.push(cur_t);
    emit("*", a, b, temp);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "additive_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}
std::tuple<NewLexeme, std::string>p33()
{
    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    NewLexeme cast_exp = LexemeS.top();
    LexemeS.pop();
    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;
    std::string b = cast_exp.realV;
    sym_tbl cur_t = tableS.top();
    tableS.pop();
    oneLexeme left = cur_t.lookup(a);
    oneLexeme right = cur_t.lookup(b);
    if (left.name == "None" && mul_exp.type != "CONSTANT" || right.name == "None" && cast_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }
    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str()) / atoi(right.value.c_str())));
    tableS.push(cur_t);
    emit("/", a, b, temp);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "additive_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}
std::tuple<NewLexeme, std::string>p34()
{
    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    NewLexeme cast_exp = LexemeS.top();
    LexemeS.pop();
    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;
    std::string b = cast_exp.realV;
    sym_tbl cur_t = tableS.top();
    tableS.pop();
    oneLexeme left = cur_t.lookup(a);
    oneLexeme right = cur_t.lookup(b);
    if (left.name == "None" && mul_exp.type != "CONSTANT" || right.name == "None" && cast_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }
    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str()) % atoi(right.value.c_str())));
    tableS.push(cur_t);
    emit("%", a, b, temp);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "additive_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}
std::tuple<NewLexeme, std::string>p37()
{
    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    NewLexeme cast_exp = LexemeS.top();
    LexemeS.pop();
    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;
    std::string b = cast_exp.realV;
    sym_tbl cur_t = tableS.top();
    tableS.pop();
    oneLexeme left = cur_t.lookup(a);
    oneLexeme right = cur_t.lookup(b);
    if (left.name == "None" && mul_exp.type != "CONSTANT" || right.name == "None" && cast_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }
    /*if (left.type != right.type) {
        wrongInfo = "wrong type";
        NewLexeme re(LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }*/
    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str()) - atoi(right.value.c_str())));
    tableS.push(cur_t);
    emit("-", a, b, temp);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "additive_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}
std::tuple<NewLexeme, std::string>p78()//declaration  --> declaration_specifiers init_declarator_list ;
{
    NewLexeme decSpec = LexemeS.top();
    LexemeS.pop();
    NewLexeme initDec = LexemeS.top();
    LexemeS.pop();
    // pop "declaration_specifiers init_declarator_list"
    std::string type = decSpec.realV;
    std::string name = initDec.realV;
    std::string value = initDec.rrealv;
    sym_tbl cur_t = tableS.top();
    tableS.pop();
    oneLexeme t(type, name, value, offset);
    cur_t.addsys(t);
    tableS.push(cur_t);
    offset += initDec.width;
    offsetS.push(offset);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "declaration", "declaration", 0);
    return std::make_tuple(pushWord, "None");
}
std::tuple<NewLexeme, std::string>p93()
{
    LexemeS.pop();

    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "type_specifier", "VOID", 0);
    pushWord.width = 4;
    return std::make_tuple(pushWord, "None");
}
std::tuple<NewLexeme, std::string>p94()
{
    LexemeS.pop();

    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "type_specifier", "CHAR", 0);
    pushWord.width = 1;
    return std::make_tuple(pushWord, "None");
}
std::tuple<NewLexeme, std::string>p95()
{
    LexemeS.pop();

    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "type_specifier", "SHORT", 0);
    pushWord.width = 2;
    return std::make_tuple(pushWord, "None");
}
std::tuple<NewLexeme, std::string>p96()
{
    LexemeS.pop();

    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "type_specifier", "INT", 0);
    pushWord.width = 4;
    return std::make_tuple(pushWord, "None");
}
std::tuple<NewLexeme, std::string>p97()
{
    LexemeS.pop();

    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "type_specifier", "LONG", 0);
    pushWord.width = 4;
    return std::make_tuple(pushWord, "None");
}
std::tuple<NewLexeme, std::string>p98()
{
    LexemeS.pop();

    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "type_specifier", "FLOAT", 0);
    pushWord.width = 4;
    return std::make_tuple(pushWord, "None");
}
std::tuple<NewLexeme, std::string>p99()
{
    LexemeS.pop();

    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "type_specifier", "DOUBLE", 0);
    pushWord.width = 8;
    return std::make_tuple(pushWord, "None");
}
std::tuple<NewLexeme, std::string>p100()
{
    LexemeS.pop();

    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "type_specifier", "SIGNED", 0);
    pushWord.width = 2;
    return std::make_tuple(pushWord, "None");
}
std::tuple<NewLexeme, std::string>p101()
{
    LexemeS.pop();

    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "type_specifier", "UNSIGNED", 0);
    pushWord.width = 2;
    return std::make_tuple(pushWord, "None");
}
std::tuple<NewLexeme, std::string>p102()
{
    LexemeS.pop();

    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "type_specifier", "struct_or_union_specifier", 0);
    pushWord.width = 4;
    return std::make_tuple(pushWord, "None");
}
std::tuple<NewLexeme, std::string>p103()
{
    LexemeS.pop();

    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "type_specifier", "enum_specifier", 0);
    pushWord.width = 4;
    return std::make_tuple(pushWord, "None");
}
std::tuple<NewLexeme, std::string>p62()
{
    NewLexeme unary_exp = LexemeS.top();
    LexemeS.pop();
    NewLexeme ass_opt = LexemeS.top();
    LexemeS.pop();
    NewLexeme ass_exp = LexemeS.top();
    LexemeS.pop();

    std::string wrongInfo = "None";

    std::string a = ass_exp.realV;

    sym_tbl cur_t = tableS.top();
    oneLexeme left = cur_t.lookup(a);

    if (left.name == "None" && ass_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        //std::cout << a << std::endl;
        return std::make_tuple(re, wrongInfo);
    }


    emit(ass_opt.realV, ass_exp.realV, "-", unary_exp.realV);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "assignment_expression", ass_exp.rrealv, 0);

    return std::make_tuple(pushWord, "None");
}
//init_declarator  --> declarator = initializer

std::tuple<NewLexeme, std::string>p88()
{
    NewLexeme unary_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    NewLexeme ass_exp = LexemeS.top();
    LexemeS.pop();
    //std::cout << ass_exp.realV << " 1" << std::endl;

    std::string wrongInfo = "None";

    std::string a = ass_exp.realV;

    sym_tbl cur_t = tableS.top();
    oneLexeme left = cur_t.lookup(a);

    if (left.name == "None" && ass_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }

    emit("=", ass_exp.realV, "-", unary_exp.realV);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "init_declarator", unary_exp.realV, 0);

    return std::make_tuple(pushWord, "None");
}
//primary_expression  --> ( expression )
std::tuple<NewLexeme, std::string>p4()
{

    LexemeS.pop();
    NewLexeme unary_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    return std::make_tuple(unary_exp, "None");
}
std::tuple<NewLexeme, std::string>p47()
{

    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    NewLexeme cast_exp = LexemeS.top();
    LexemeS.pop();
    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;
    std::string b = cast_exp.realV;
    sym_tbl cur_t = tableS.top();
    tableS.pop();
    oneLexeme left = cur_t.lookup(a);
    oneLexeme right = cur_t.lookup(b);
    if (left.name == "None" && mul_exp.type != "CONSTANT" || right.name == "None" && cast_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }
    /*if (left.type != right.type) {
        wrongInfo = "wrong type";
        NewLexeme re(LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }*/
    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str())== atoi(right.value.c_str())));
    tableS.push(cur_t);
    emit("==", a, b, temp);
    std::cout << temp;
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "relational_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}
std::tuple<NewLexeme, std::string>p48()
{

    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    NewLexeme cast_exp = LexemeS.top();
    LexemeS.pop();
    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;
    std::string b = cast_exp.realV;
    sym_tbl cur_t = tableS.top();
    tableS.pop();
    oneLexeme left = cur_t.lookup(a);
    oneLexeme right = cur_t.lookup(b);
    if (left.name == "None" && mul_exp.type != "CONSTANT" || right.name == "None" && cast_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }
    /*if (left.type != right.type) {
        wrongInfo = "wrong type";
        NewLexeme re(LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }*/
    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str()) != atoi(right.value.c_str())));
    tableS.push(cur_t);
    emit("!=", a, b, temp);
    std::cout << temp;
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "relational_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}

std::tuple<NewLexeme, std::string>p42()
{

    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    NewLexeme cast_exp = LexemeS.top();
    LexemeS.pop();
    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;
    std::string b = cast_exp.realV;
    sym_tbl cur_t = tableS.top();
    tableS.pop();
    oneLexeme left = cur_t.lookup(a);
    oneLexeme right = cur_t.lookup(b);
    if (left.name == "None" && mul_exp.type != "CONSTANT" || right.name == "None" && cast_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }
    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str()) < atoi(right.value.c_str())));
    tableS.push(cur_t);
    emit("<", a, b, temp);

    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "relational_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}

std::tuple<NewLexeme, std::string>p43()
{

    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    NewLexeme cast_exp = LexemeS.top();
    LexemeS.pop();
    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;
    std::string b = cast_exp.realV;
    sym_tbl cur_t = tableS.top();
    tableS.pop();
    oneLexeme left = cur_t.lookup(a);
    oneLexeme right = cur_t.lookup(b);
    if (left.name == "None" && mul_exp.type != "CONSTANT" || right.name == "None" && cast_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }
    /*if (left.type != right.type) {
        wrongInfo = "wrong type";
        NewLexeme re(LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }*/
    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str()) > atoi(right.value.c_str())));
    tableS.push(cur_t);
    emit(">", a, b, temp);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "relational_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}
std::tuple<NewLexeme, std::string>p44()
{

    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    NewLexeme cast_exp = LexemeS.top();
    LexemeS.pop();
    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;
    std::string b = cast_exp.realV;
    sym_tbl cur_t = tableS.top();
    tableS.pop();
    oneLexeme left = cur_t.lookup(a);
    oneLexeme right = cur_t.lookup(b);
    if (left.name == "None" && mul_exp.type != "CONSTANT" || right.name == "None" && cast_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }
    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str()) <= atoi(right.value.c_str())));
    tableS.push(cur_t);
    emit("<=", a, b, temp);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "relational_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}
std::tuple<NewLexeme, std::string>p45()
{

    NewLexeme mul_exp = LexemeS.top();
    LexemeS.pop();
    LexemeS.pop();
    NewLexeme cast_exp = LexemeS.top();
    LexemeS.pop();
    std::string wrongInfo = "None";

    std::string a = mul_exp.realV;
    std::string b = cast_exp.realV;
    sym_tbl cur_t = tableS.top();
    tableS.pop();
    oneLexeme left = cur_t.lookup(a);
    oneLexeme right = cur_t.lookup(b);
    if (left.name == "None" && mul_exp.type != "CONSTANT" || right.name == "None" && cast_exp.type != "CONSTANT") {
        wrongInfo = "SEMANTIC ERROR::use undeclared variable\n";
        NewLexeme re(LINEOFNONT, LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }
    /*if (left.type != right.type) {
        wrongInfo = "wrong type";
        NewLexeme re(LINEOFNONT, "error", "error", -1);
        return std::make_tuple(re, wrongInfo);
    }*/
    std::string temp = cur_t.newTemp(left.type, std::to_string(atoi(left.value.c_str()) >= atoi(right.value.c_str())));
    tableS.push(cur_t);
    emit(">=", a, b, temp);
    NewLexeme pushWord(LINEOFNONT, LINEOFNONT, "relational_expression", temp, 0);
    return std::make_tuple(pushWord, wrongInfo);
}


//构造函数
mid_code_gen_parser::mid_code_gen_parser(){
    sym_tbl all_t("global");
    tableS.push(all_t);
    nextquad = 0;
    for (int i = 0; i < 229; i++)
        p[i] = Default_expression;

    //assign & declarations
    p[78] = p78;
    p[2] = p2;
    p[62] = p62;
    p[88] = p88;
    p[93] = p93;
    p[94] = p94;
    p[95] = p95;
    p[96] = p96;
    p[97] = p97;
    p[98] = p98;
    p[99] = p99;
    p[100] = p100;
    p[101] = p101;
    p[102] = p102;
    p[103] = p103;
    p[36] = p36;
    p[32] = p32;
    p[33] = p33;
    p[34] = p34;
    p[37] = p37;
    p[11] = p11;
    p[12] = p12;
    p[4] = p4;
    p[42] = p42;
    p[43] = p43;
    p[44] = p44;
    p[45] = p45;
    p[47] = p47;
    p[48] = p48;

    //control & bool
    p[58] = OR_expression;
    p[56] = AND_expression;
    p[20] = NOT_expression;
    p[4] = Identifier_expression;
    p[42] = Relop_expression;
    p[43] = Relop_expression;
    p[44] = Relop_expression;
    p[45] = Relop_expression;
    p[47] = Relop_expression;
    p[48] = Relop_expression;
    p[207] = if_;
    p[208] = if_else;
    p[210] = while_;
    p[206] = end_sentence;
    p[205] = end_sentence_2;
}

std::tuple<bool, std::string, int, int> mid_code_gen_parser::check(const std::string path)
{
    //变量定义
    unsigned int max_num=0;




    int nodeID = 0;
    int eplisonID = 2147483647;
    std::fstream f;
    f.open(path, std::ios::in);
    std::fstream of;
    of.open("./resI.dot", std::ios::out);
    std::fstream tmp_file;
    tmp_file.open("stack.tmp",std::ios::out);
    //输出中间函数的变量
    std::fstream f1;
    f1.open("IR.txt",std::ios::out);



    while (!itemS.empty())
        itemS.pop();
    while (!symbolS.empty())
        symbolS.pop();

    itemS.push(0);// I0 into stack
    symbolS.push(NewLexeme(-1,-1,"#","#", -1));

    tmp_file<<"p"<<" "<<0<<" "<<"#\n";

    of << "digraph G{\ngraph[dpi=300,autosize=false,size=\"200, 200\"];\noverlap=false; \nspines=true;\nnode[ shape=\"box\"];\n";

    while (f.eof() == false)
    {
        bool findFLAG = false, R_flag = false;
        unsigned int pos=0, pos1=0, pos2 = 0;
        std::string tmp;

        getline(f, tmp);
        pos = tmp.find_first_of(' ');
        pos1 = tmp.find_first_of(' ', pos + 1);
        pos2 = tmp.find_first_of(' ', pos1 + 1);

        NewLexeme inputw(atoi(tmp.substr(0, pos - 0).c_str()), atoi(tmp.substr(pos + 1, pos1 - pos - 1).c_str()), tmp.substr(pos1 + 1, pos2 - pos1 - 1), tmp.substr(pos2 + 1, tmp.size() - pos2 - 1), nodeID);

        tmp_file<<"@ "<<inputw.value<<"\n";
        int topState = itemS.top();

        Action* a_ptr = LR1_table[topState].action_ptr->next;

        //重新定义
        bool syntax_tag = false;
        bool semantic_tag = true;
        int error_line = 0, error_col = 0;
        std::string error_message;

        while (a_ptr)
        {
            R_flag = false;
            if (a_ptr->symbol_name == inputw.value){
                syntax_tag = true;
                if (a_ptr->dst == ACC)
                {
                    of << "}\n";
                    for (int i = 0; i < QuadrupleForm.size(); i++)
                    {
                        auto it = QuadrupleForm[i];
                        f1 << i << ' ' << "( " << it.type << ", " << it.a << ", " << it.b << ", " << it.addr << " )" << std::endl;
                    }
                    //4
                    tmp_file<<"b\n";
                    tmp_file<<"#\n";
                    CLOSE_FILES
                    return std::make_tuple(true, "none", 0, 0);
                }
                findFLAG = true;
                if (a_ptr->dst < 0)
                {//s
                    itemS.push(-a_ptr->dst);
                    nodeID++;
                    inputw.setID(nodeID);

                    //新加入
                    if (inputw.value == "ELSE")
                    {//"else"——special_judge to replace N
                        makelist(nextquad, &(inputw.nextlist));
                        emit("j", "None", "None", "INIT_STATE");
                    }


                    symbolS.push(inputw);

                    //7
                    tmp_file<<"p "<<-a_ptr->dst<<" "<<inputw.value<<"\n";


                    //2
                    if(itemS.size()>max_num)
                        max_num=itemS.size();
                    break;
                }
                else if (a_ptr->dst > 0)
                {//r
                    //
                    syntax_tag = false;
                    ProductionRule x=pure_generator_list[a_ptr->dst];
                    //
                    nodeID++;
                    int fid=nodeID;
//                    NewLexeme fword(LINEOFNONT, x.left.name, x.left.name, fid);
                    NewLexeme fword;
                    std::string type;
                    bool isTrans = false;

                    of << "node" << nodeID << "[label=\"" << x.left.name << "\"]\n";
                    /*-----------------------------*/
                    if (x.right_list.at(0).name != "$") {
                        int n = x.right_list.size();
                        for (int i = 0; i < n; i++) {

                            NewLexeme topW=symbolS.top();

                            //!检测部分
                            if (n == 1)
                            {
                                fword = topW;
                                type = topW.type;
                                isTrans = true;
                            }
                            //
                            if (p[a_ptr->dst] != Default_expression)
                            {
                                LexemeS.push(topW);
                            }
                            //LexemeS.push(topW);
                            //!


                            //std::cout << topW.id << "\n";
                            if(topW.line!=LINEOFNONT)
                                of << "node" << topW.id << "[label=\"" << topW.value<<"\\n"<<topW.realV << "\"]\n";
                            of << "node" << fid << "->node" << topW.id << "\n";

                            symbolS.pop();
                            itemS.pop();

                            //8
                            tmp_file<<"b\n";
                        }
                    }
                    else
                    {
                        eplisonID--;
                        of << "node" << eplisonID << "[label=\"Eplison\"]\n";
                        of << "node" << fid << "->node" << eplisonID << "\n";
                    }/**/
                    topState = itemS.top();
                    //加入语义分析
                    if (p[a_ptr->dst] != Default_expression) {
                        auto fx = p[a_ptr->dst]();
                        fword = std::get<0>(fx);

                        //test
                        string message = std::get<1>(fx);
                        if (message != "None")
                        {
                            error_message = message;
                            semantic_tag = false;
                            error_col = inputw.col;
                            error_line = inputw.line;
                            std::cout << "123333" << ' ' << error_line << ' ' << error_col << ' ' << message << "\n";
                            break;
                        }
                        std::cout << error_line << ' ' << error_col << ' ' << message << "\n";

                        //auto [fword,wronginfo] = p[a_ptr->dst]();
                    }
                    else
                    {
                        fword.value = x.left.name;
                        fword.type = type;
                        if (fword.quad == "")//to prevent S/E has no attributes(quad,nextlist...) like if(a){ int b;int c;} (S1 has no attribute)
                            fword.quad = std::to_string(nextquad - 1);
                        if (fword.nextlist.empty())
                            makelist(nextquad - 1, &fword.nextlist);
                        //std::cout << fword.value<<" "<< fword.type<<std::endl;
                        if (!isTrans)
                            fword = NewLexeme(LINEOFNONT, LINEOFNONT, x.left.name, x.left.name, fid);
                        else
                        {
                            //fword.value = x.left.name;
                            fword.id = fid;
                        }
                    }
                    fword.setID(fid);
                    fword.value = x.left.name;

                    symbolS.push(fword);


                    StateTransition* g_ptr = LR1_table[topState].goto_ptr->next;
                    while (g_ptr)
                    {
                        if (g_ptr->symbol_name == x.left.name)
                        {
                            syntax_tag = true;
                            R_flag = true;
                            itemS.push(g_ptr->dst);

                            //9
                            tmp_file<<"p "<<g_ptr->dst<<" "<<fword.value<<"\n";
                            //3

                            break;
                        }
                        g_ptr = g_ptr->next;
                    }
                    if (R_flag == true) {
                        topState = itemS.top();
                        a_ptr = LR1_table[topState].action_ptr->next;
                        continue;
                    }
                    break;
                }
            }
            a_ptr = a_ptr->next;
        }

        if (a_ptr == nullptr)
        {
            syntax_tag = false;
            tmp_file.close();
            CLOSE_FILES
            return std::make_tuple(false, "SYNTAX ERROR::syntax error\n", inputw.line, inputw.col);
        }
        if (semantic_tag == false)
        {
            tmp_file.close();
            CLOSE_FILES
            return std::make_tuple(false, error_message, error_line, error_col);
        }
        if (syntax_tag == false)
        {
            tmp_file.close();
            CLOSE_FILES
            return std::make_tuple(false, "SYNTAX ERROR::syntax error\n", inputw.line, inputw.col);
        }

        if (findFLAG == false&&R_flag==false) {
            //of << "}\n";

            ///5
            tmp_file.close();
            CLOSE_FILES
            return std::make_tuple(false, "SYNTAX ERROR::syntax error\n", inputw.line, inputw.col);
        }
    }

}
