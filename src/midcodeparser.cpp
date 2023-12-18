
#include "midcodeparser.h"
#include "production.h"
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <queue>
#include <stack>
#include <sstream>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#define test
#define SUCCESS 0
#define ERROR -1

#include <iostream>
#include "Lexeme.h"
using namespace std;

std::stack<std::string> strStackTest;
std::stack<std::string> varStackTest;
int tempNumTest;
int labelNumTest;

ofstream fout;
//四元式结构体
std::vector<Quaternion> QuadrupleForm;//TODO:或者直接push 就以全局的nextquad为下标进行存储



std::string itoTempTest(int i)
{
    std::ostringstream temp;
    temp << i << "_TEMP";
    return temp.str();
}

std::string itoLabelTest(int i)
{
    std::ostringstream temp;
    temp << i << "_LABEL";
    return temp.str();
}

std::string varStackPopTest()
{
    auto var = varStackTest.top();
    varStackTest.pop();
    return var;
}

std::string strStackPopTest()
{
    auto str = strStackTest.top();
    strStackTest.pop();
    return str;
}

std::string cmd(std::string a, std::string b, std::string c, std::string d)
{
    QuadrupleForm.push_back(Quaternion(a,b,c,d));
    return  "(" + a + ", " + b + ", " + c + ", " + d + ")";
}

bool _IsDigit(const string a)
{
    for (auto it : a)
        if (!isdigit(it))
            return false;
    return true;
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

int MidCodeParser::translate(int id, std::string name)
{
#ifdef test
    cout << "[translate] " << id << "  varSize=[" << varStackTest.size() << "]";
    cout << "    strSize=[" << strStackTest.size() << "]" << endl;

    fout << "[translate] " << id << "  varSize=[" << varStackTest.size() << "]";
    fout << "    strSize=[" << strStackTest.size() << "]" << endl;
#endif // test
    switch (id)
    {
    case 3:
    {
        varStackPopTest();
        varStackPopTest();
#ifdef test
        cout << "[case 3] [var pop]" << endl;
        cout << "[case 3] [var pop]" << endl;

        fout << "[case 3] [var pop]" << endl;
        fout << "[case 3] [var pop]" << endl;
#endif // test
        break;
    }

    case 9: case 10:
    {
        auto str = strStackPopTest();
        std::ostringstream os;
        os << std::endl << str;
        strStackTest.push(os.str());
        break;
    }

    case 13:
    {
        //构造符号表

        varStackPopTest();
#ifdef test
        cout << "[case 13] [var pop]" << endl;
        fout << "[case 13] [var pop]" << endl;
#endif // test
        break;
    }

    case 15:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        strStackTest.push(os.str());
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        varStackTest.push(lvar);
#ifdef test
        cout << "[case 15] [str pop] " << endl;
        cout << "[case 15] [str pop] " << endl;
        cout << "[case 15] [str push] " << os.str() << endl;

        fout << "[case 15] [str pop] " << endl;
        fout << "[case 15] [str pop] " << endl;
        fout << "[case 15] [str push] " << os.str() << endl;
#endif // test
        break;
    }

    case 24:
    {
        auto r = varStackPopTest();
        auto l = varStackPopTest();
        //检查是否符号表，类型是否正确，是否定义


        std::ostringstream os;
        os << strStackPopTest();
        os << cmd("=", r, "", l) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(l);
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 27:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmd("or", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 29:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmd("and", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 31:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmd("==", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 32:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmd("!=", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 34:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmd("<", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 35:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmd(">", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 36:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmd("<=", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 37:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmd(">=", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 39:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmd("+", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 40:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmd("-", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 42:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmd("*", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 43:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        os << lstr << rstr;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << cmd("/", lvar, rvar, itoTempTest(tempNumTest)) << std::endl;
        strStackTest.push(os.str());
        varStackTest.push(itoTempTest(tempNumTest));
        tempNumTest++;
#ifdef test
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str pop] " << endl;
        cout << "[case" << id << "] [str push] " << os.str() << endl;

        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str pop] " << endl;
        fout << "[case" << id << "] [str push] " << os.str() << endl;
#endif // test
        break;
    }
    case 44: case 45: case 46: case 47:
    {
        strStackTest.push("");
#ifdef test
        cout << "[case" << id << "] [str push] []" << endl;

        fout << "[case" << id << "] [str push] []" << endl;
#endif // test
        break;
    }

    case 49:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();

        os << cmd(itoLabelTest(labelNumTest), "", "", "") << std::endl;
        os << lstr;
        os << cmd("J!=", lvar, "0", itoLabelTest(labelNumTest + 1)) << std::endl;
        os << cmd("J", "", "", itoLabelTest(labelNumTest + 2)) << std::endl;
        os << cmd(itoLabelTest(labelNumTest + 1), "", "", "") << std::endl;
        os << rstr;
        os << cmd("J", "", "", itoLabelTest(labelNumTest)) << std::endl;
        os << cmd(itoLabelTest(labelNumTest + 2), "", "", "") << std::endl;
        labelNumTest += 3;
        strStackTest.push(os.str());
        varStackTest.push(lvar);
        break;
    }

    case 52:
    {
        auto rstr = strStackPopTest();
        auto lstr = strStackPopTest();
        std::ostringstream os;
        auto rvar = varStackPopTest();
        auto lvar = varStackPopTest();
        os << lstr;
        os << cmd("J!=", lvar, "0", itoLabelTest(labelNumTest)) << std::endl;
        os << cmd("J", "", "", itoLabelTest(labelNumTest + 1)) << std::endl;
        os << cmd(itoLabelTest(labelNumTest), "", "", "") << std::endl;;
        os << rstr;
        os << cmd(itoLabelTest(labelNumTest + 1), "", "", "") << std::endl;
        labelNumTest += 2;
        strStackTest.push(os.str());
        varStackTest.push(lvar);
        break;
    }

    case 53:
    {
        auto rstr = strStackPopTest();
        auto midstr = strStackPopTest();
        auto lstr = strStackPopTest();

        std::ostringstream os;
        auto rvar = varStackPopTest();
        auto midvar = varStackPopTest();
        auto lvar = varStackPopTest();

        os << lstr;
        os << cmd("J!=", lvar, "0", itoLabelTest(labelNumTest)) << std::endl;
        os << cmd("J", "", "", itoLabelTest(labelNumTest + 1)) << std::endl;
        os << cmd(itoLabelTest(labelNumTest), "", "", "") << std::endl;;
        os << midstr;
        os << cmd("J", "", "", itoLabelTest(labelNumTest + 2)) << std::endl;
        os << cmd(itoLabelTest(labelNumTest + 1), "", "", "") << std::endl;
        os << rstr;
        os << cmd(itoLabelTest(labelNumTest + 2), "", "", "") << std::endl;
        labelNumTest += 3;
        strStackTest.push(os.str());
        varStackTest.push(lvar);
        break;
    }

    default:
        break;
    }
#ifdef test
    cout << "[/translate] " << id << "  varSize=[" << varStackTest.size() << "]";
    cout << "    strSize=[" << strStackTest.size() << "]" << endl;
    cout << endl;

    fout << "[/translate] " << id << "  varSize=[" << varStackTest.size() << "]";
    fout << "    strSize=[" << strStackTest.size() << "]" << endl;
    fout << endl;
#endif // test
    return 1;
}

// analyze the tokens with grammar
int MidCodeParser::analyse(const std::vector<Token>& tokens)
{
    tempNumTest = 0;
    labelNumTest = 0;
    build();

    std::cout << std::endl;
    std::ofstream stackOut("stack.txt", ios::out);
    if (stackOut.is_open() == 0)
    {
        cout << "output sysstack err" << endl;
        exit(-1);
    }


    fout << std::endl;

    fout.open("Analyze.txt", ios::out);
    if (fout.is_open() == 0)
    {
        cout << "output open failed" << endl;

        fout << "output open failed" << endl;
        exit(-1);
    }

    std::stack<std::pair<unsigned int, std::string>> st;
    st.push({ 0,"$" });
    auto iter = tokens.cbegin();

    //在此输出到文件
    stackOut << "当前第一字符:" << std::endl;
    stackOut << iter->get_name() << " " << iter->get_type_output() << std::endl;

    for (; ; )
    {
        auto I = st.top().first;
        std::string type;
        if (iter->get_type() == TokenType::ID || iter->get_type() == TokenType::CHAR
            || iter->get_type() == TokenType::INT || iter->get_type() == TokenType::FLOAT)
            type = TokenDict[iter->get_type()];
        else
            type = iter->get_name();

        if (action[I].find(type) != action[I].end())
        {
            auto act = action[I][type];
            if (act.first == "S")
            {
                if (iter->get_type() == TokenType::ID || iter->get_type() == TokenType::CHAR
                    || iter->get_type() == TokenType::INT || iter->get_type() == TokenType::FLOAT)
                {
                    varStackTest.push(iter->get_name());
                    //cout<<"[pushVAR] "<<iter->get_name() <<"   [at line "<<iter->get_line()<<"]"<<endl;
                }
                st.push({ act.second, type });

                //在这里输出程序运行中栈的信息
                //                std::cout << "Stack Information:" << std::endl;
                //                for (std::stack<std::pair<unsigned int, std::string>> dump = st; !dump.empty(); dump.pop())
                //                    std::cout << dump.top().first << " " << dump.top().second << '\n';
                //                std::cout << std::endl;
                stackOut << "当前操作：入栈\n";
                stackOut << "栈内信息:" << std::endl;
                for (std::stack<std::pair<unsigned int, std::string>> dump = st; !dump.empty(); dump.pop())
                    stackOut << dump.top().first << " " << dump.top().second << '\n';
                stackOut << std::endl;

                //system("pause");

                iter++;
                //                //在这里输出程序运行中插入token的信息
                //                std::cout << "Inserted element:" << std::endl;
                //                std::cout << iter->get_name() << " " << iter->get_type_output() << std::endl;
                stackOut<< "当前第一字符:" << std::endl;
                stackOut << iter->get_name() << " " << iter->get_type_output() << std::endl;


            }
            else if (act.first == "r")
            {
                auto id = act.second;
                auto right = grammar[id].get_right();
                for (unsigned int i = 0; i < right.size(); i++)
                    if (right[i] != "@")
                        st.pop();
                auto newI = st.top().first;
                st.push({ go[newI][grammar[id].get_left()],grammar[id].get_left() });

// translate
#ifdef test
                cout<<"use production["<<id<<"]: "<<grammar[id].get_left()<<"->";
                for(auto e:right)cout<<e<<" ";
                cout<<endl;

                fout<<"use production["<<id<<"]: "<<grammar[id].get_left()<<"->";
                for(auto e:right)fout<<e<<" ";
                fout<<endl;
#endif // test
                if (!translate(id, iter->get_name()))
                {
                    std::cout << "ERROR! at line " << iter->get_line() << std::endl;
                    fout << "ERROR! at line " << iter->get_line() << std::endl;
                    return iter->get_line();
                }

                stackOut << "当前操作：归约\n";
                stackOut << "栈内信息:" << std::endl;
                    for (std::stack<std::pair<unsigned int, std::string>> dump = st; !dump.empty(); dump.pop())
                    stackOut << dump.top().first << " " << dump.top().second << '\n';
                stackOut << std::endl;

                stackOut<< "当前第一字符:" << std::endl;
                stackOut << iter->get_name() << " " << iter->get_type_output() << std::endl;
            }

            else if (act.first == "acc")
            {

                ofstream fout;
                fout.open("Quaternion_information.txt", ios::out);
                if (fout.is_open() == 0)
                {
                    cout << "output open failed" << endl;
                    fout << "output open failed" << endl;
                    exit(-1);
                }

                std::cout << "Accept!" << std::endl;
                fout << "Accept!" << std::endl;

                //在这里输出最后的四元式
                std::cout << "Quaternion information:" << std::endl;
                std::cout << strStackTest.top() << std::endl;

                fout << "Quaternion information:" << std::endl;
                fout << strStackTest.top() << std::endl;

                fout.close();
                stackOut.close();
                stackOut << "当前操作：终止\n";
                stackOut << "栈内信息:" << std::endl;
                    for (std::stack<std::pair<unsigned int, std::string>> dump = st; !dump.empty(); dump.pop())
                    stackOut << dump.top().first << " " << dump.top().second << '\n';
                stackOut << std::endl;

                return 0;
            }
        }

        else
        {
            std::cout << "ERROR! at line " << iter->get_line() << std::endl;
            fout << "ERROR! at line " << iter->get_line() << std::endl;
            return iter->get_line();
        }
    }
    return 0;
}

// get one symbol's first set
//std::set<std::string> MidCodeParser::get_first(const std::string& symbol)
//{
//    if (first_map.find(symbol) != first_map.end())         // have this set, then return
//        return first_map[symbol];
//    std::set<std::string> ret;
//    if (termi_set.find(symbol) != termi_set.end())   // is a terminal symbol
//    {
//        ret.insert(symbol);
//        return ret;
//    }
//    else                                                // is a variable symbol
//    {
//        for (auto production : grammar)
//        {
//            if (production.get_left() == symbol)
//            {
//                auto right = production.get_right();
//                if (right.size() == 1 && right[0] == "@")  //insert @
//                {
//                    ret.insert("@");
//                    continue;
//                }
//                else if (right.size() > 0                //insert the first terminal symbol
//                         && termi_set.find(right[0]) != termi_set.end())
//                {
//                    ret.insert(right[0]);
//                    continue;
//                }
//            }
//        }
//    }
//    return ret;
//}

//// build the first Map, first_map[x] = first(x)
//void MidCodeParser::gen_first_map()
//{
//    first_map.clear();

//    // basic
//    for (auto symbol : termi_set)
//        first_map.insert({ symbol, get_first(symbol) });
//    for (auto symbol : var_set)
//        first_map.insert({ symbol, get_first(symbol) });

//    // merge
//    for (; ; )
//    {
//        bool changed = false;
//        for (auto symbol : var_set)
//        {
//            //if(symbol == "ADD")cout<<"get first (add) "<<endl;
//            for (;;)
//            {
//                auto temp = first_map[symbol];
//                //if(symbol == "ADD"){cout<<"first= ";for(auto e:temp)cout<<e<<" ";cout<<endl;}
//                for (auto produciton : grammar)
//                {
//                    if (produciton.get_left() == symbol)
//                    {
//                        bool allEmpty = true;
//                        auto right = produciton.get_right();
//                        //if(symbol == "ADD"){cout<<"  right= ";for(auto e:right)cout<<e<<" ";cout<<endl;}
//                        for (auto one : right)
//                        {
//                            auto oneSet = first_map[one];
//                            //if(symbol == "ADD"){cout<<"  oneset= ";for(auto e:oneSet)cout<<e<<" ";cout<<endl;}
//                            if (oneSet.erase("@"))
//                            {
//                                temp.insert(oneSet.begin(), oneSet.end());
//                            }
//                            else
//                            {
//                                temp.insert(oneSet.begin(), oneSet.end());
//                                allEmpty = false;
//                                break;
//                            }
//                        }
//                        if (allEmpty)
//                            temp.insert("@");
//                    }
//                }
//                //if(symbol == "ADD"){cout<<" now tot = ";for(auto e:temp)cout<<e<<" ";cout<<endl;}
//                if (temp == first_map[symbol])
//                    break;
//                first_map[symbol] = temp;
//                changed = true;
//            }
//        }
//        if (!changed)
//            break;
//    }
//}

//// get first(beta), beta = A1A2...An
//std::set<std::string> MidCodeParser::get_first(const std::vector<std::string>& beta)
//{
//    std::set<std::string> ret;
//    bool allEmpty = true;
//    for (auto one : beta)
//    {
//        auto oneSet = first_map[one];
//        if (oneSet.erase("@"))
//        {
//            ret.insert(oneSet.begin(), oneSet.end());
//        }
//        else
//        {
//            ret.insert(oneSet.begin(), oneSet.end());
//            allEmpty = false;
//            break;
//        }
//    }
//    if (allEmpty)
//        ret.insert("@");
//    return ret;
//}

//// build action and goto table
//void MidCodeParser::build()
//{
//    if (closure_map.size() != 0)                      // have built action and goto table
//        return;

//    gen_first_map();

//    auto closure0 = get_closure(LR1(LR0(0, 0), "$"));

//    std::queue<LR1set> q;
//    q.push(closure0);

//    //closure_map[closure0] = closure_map.size();     // first insert closure, then map[closure] = 1

//    closure_list.push_back(closure0);
//    closure_map.insert(make_pair(closure0, closure_map.size()));


//    while (!q.empty())
//    {
//        auto lr1set = q.front();
//        q.pop();
//        transfer.push_back(vpsi());

//        for (auto variable : var_set)
//        {
//            LR1set newset;
//            for (auto lr1 : lr1set)
//            {
//                auto lr0 = lr1.get_left();
//                auto id = lr0.get_left();
//                auto pointpos = lr0.get_right();
//                if (grammar[id].get_right().size() > pointpos && grammar[id].get_right()[pointpos] == variable)
//                    newset.insert(LR1(LR0(id, pointpos + 1), lr1.get_right()));
//            }
//            if (newset.size() != 0)
//            {
//                get_closure(newset);
//                if (closure_map.find(newset) == closure_map.end())
//                {
//                    closure_list.push_back(newset);
//                    closure_map.insert(make_pair(newset, closure_map.size()));
//                    //closure_map[newset]=closure_map.size();
//                    q.push(newset);
//                }
//                transfer[transfer.size() - 1].push_back(make_pair(variable, closure_map[newset]));
//            }
//        }

//        for (auto terminal : termi_set)
//        {
//            LR1set newset;
//            for (auto lr1 : lr1set)
//            {
//                auto lr0 = lr1.get_left();
//                auto id = lr0.get_left();
//                auto pointpos = lr0.get_right();
//                if (grammar[id].get_right().size() != pointpos && grammar[id].get_right()[pointpos] == terminal)
//                    newset.insert(LR1(LR0(id, pointpos + 1), lr1.get_right()));
//            }
//            if (newset.size() != 0)
//            {
//                get_closure(newset);
//                if (closure_map.find(newset) == closure_map.end())
//                {
//                    closure_list.push_back(newset);
//                    closure_map.insert(make_pair(newset, closure_map.size()));
//                    //closure_map[newset]=closure_map.size();
//                    q.push(newset);
//                }
//                transfer[transfer.size() - 1].push_back(std::make_pair(terminal, closure_map[newset]));
//            }
//        }
//    }

//    for (unsigned int i = 0; i < closure_list.size(); i++)
//    {
//        action.push_back(std::map<std::string, psi>());
//        go.push_back(std::map<std::string, int>());

//        for (auto lr1 : closure_list[i])
//        {
//            auto point = lr1.get_left().get_right();
//            auto right = grammar[lr1.get_left().get_left()].get_right();
//            if (point < right.size() && termi_set.find(right[point]) != termi_set.end())
//            {
//                for (auto go : transfer[i])
//                    if (go.first == right[point])
//                    {
//#ifdef test
//                        if (action[i].find(right[point]) != action[i].end()
//                                && action[i][right[point]] != pair<string, int>("S", go.second))
//                        {
//                            cout << "error 1 at action[" << i << "][" << right[point] << "]=S" << go.second;
//                            cout << "  old=" << action[i][right[point]].first << action[i][right[point]].second << endl;

//                            fout << "error 1 at action[" << i << "][" << right[point] << "]=S" << go.second;
//                            fout << "  old=" << action[i][right[point]].first << action[i][right[point]].second << endl;
//                        }
//#endif // test
//                        action[i][right[point]] = std::make_pair("S", go.second);
//                    }
//            }
//        }
//        for (auto tf : transfer[i])
//        {
//            if (var_set.find(tf.first) != var_set.end())
//            {
//#ifdef test
//                if (go[i].find(tf.first) != go[i].end())
//                {
//                    cout << "error at go[" << i << "][" << tf.first << "]" << endl;
//                    fout << "error at go[" << i << "][" << tf.first << "]" << endl;
//                }
//#endif // test
//                go[i][tf.first] = tf.second;

//            }
//        }
//        for (auto lr1 : closure_list[i])
//        {
//            auto lookahead = lr1.get_right();
//            auto lr0 = lr1.get_left();
//            auto id = lr0.get_left();
//            auto point = lr0.get_right();
//            auto right = grammar[id].get_right();

//            if (point == right.size())
//            {
//#ifdef test
//                if (action[i].find(lookahead) != action[i].end()
//                        && action[i][lookahead] != pair<string, int>("r", id))
//                {
//                    cout << "error 2 at action[" << i << "][" << lookahead << "]=r" << id << " old=" << action[i][lookahead].first << action[i][lookahead].second << std::endl;
//                    fout << "error 2 at action[" << i << "][" << lookahead << "]=r" << id << " old=" << action[i][lookahead].first << action[i][lookahead].second << std::endl;
//                }
//#endif
//                // deal with if else / if
//                if (!(action[i].find(lookahead) != action[i].end()
//                      && action[i][lookahead] != std::pair<std::string, int>("r", id)))
//                    action[i][lookahead] = std::make_pair("r", id);

//                if (lookahead == "$" && id == 0)
//                {
//                    action[i][lookahead] = std::make_pair("acc", 0);
//                }
//            }
//        }
//    }
//}

//// open grammar.txt, read grammar
//bool MidCodeParser::open_file(const std::string& fileName)
//{
//    std::ifstream in(fileName);

//    if (in)
//    {
//        std::string str;
//        int type = 0;
//        while (in >> str)
//        {
//            if (str == "[terminal]")
//            {
//                type = 1;
//                continue;
//            }
//            else if (str == "[variable]")
//            {
//                type = 2;
//                continue;
//            }
//            else if (str == "[production]")
//                break;
//            else
//            {
//                if (type == 1)
//                    termi_set.insert(str);
//                else if (type == 2)
//                    var_set.insert(str);
//            }
//        }

//        while (getline(in, str))
//        {
//            if (str.empty())
//                continue;
//            std::string temp;
//            std::istringstream iss = std::istringstream(str);
//            Production production;
//            iss >> temp;
//            iss >> temp;
//            production.set_left(temp);
//            while (iss >> temp)
//            {
//                if (temp == "->")
//                    continue;
//                production.add_right(temp);
//            }
//#ifdef test
//            /*
//            cout<<production.get_left()<<"->";
//            for(auto e:production.get_right())
//              cout<<e<<" ";
//            cout<<endl;
//            */
//#endif // test
//            grammar.push_back(production);
//        }
//        return true;
//    }
//    else
//    {
//        std::cerr << fileName << "开启失败" << std::endl;
//        return false;
//    }
//}

//// get grammar
//std::vector<Production> MidCodeParser::read_grammer()
//{
//    return grammar;
//}

//void MidCodeParser::get_closure(LR1set& closure)
//{
//    std::set<LR1> temp;

//    for (;;)
//    {
//        temp = closure;

//        for (auto lr1 : closure)
//        {
//            LR0 lr0 = lr1.get_left();
//            std::string lookahead = lr1.get_right();

//            unsigned int pointPos = lr0.get_right();
//            auto right = grammar[lr0.get_left()].get_right();

//            if (pointPos != right.size() && var_set.find(right[pointPos]) != var_set.end())
//            {
//                std::string B = right[pointPos];

//                std::vector<std::string> beta;
//                for (unsigned int i = pointPos + 1; i < right.size(); i++)
//                    beta.push_back(right[i]);
//                beta.push_back(lookahead);

//                auto first = get_first(beta);

//                for (unsigned int productionID = 0; productionID < grammar.size(); productionID++)
//                {
//                    const Production& production = grammar[productionID];
//                    if (production.get_left() == B)
//                    {
//                        for (auto terminal : first)
//                        {
//                            LR1 newItem;
//                            if (production.get_right().size() == 1 && production.get_right()[0] == "@")
//                                newItem = LR1(LR0(productionID, 1), terminal);
//                            else
//                                newItem = LR1(LR0(productionID, 0), terminal);
//                            if (closure.find(newItem) == closure.end())
//                                temp.insert(newItem);
//                        }
//                    }
//                }
//            }
//        }
//        if (temp == closure)
//            break;
//        else
//            closure = temp;
//    }
//}
//// get a closure set with LR1 item
//std::set<LR1> MidCodeParser::get_closure(const LR1& item)
//{
//    std::set<LR1> closure = { item };
//    get_closure(closure);
//    return closure;
//}

//// get the closure


//// get closure list
//std::vector< std::set<LR1> > MidCodeParser::get_closure()
//{
//    return closure_list;
//}

//// get closure map
//std::map< std::set<LR1>, int> MidCodeParser::get_closure_map()
//{
//    return closure_map;
//}

//// get transfer
//std::vector< vpsi > MidCodeParser::get_transfer()
//{
//    return transfer;
//}

//// get action table
//std::vector< std::map<std::string, psi>> MidCodeParser::get_action()
//{
//    return action;
//}

//// get goto table
//std::vector< std::map<std::string, int>> MidCodeParser::get_go()
//{
//    return go;
//}

//// get variable symbol set
//std::set< std::string > MidCodeParser::get_var()
//{
//    return var_set;
//}

//// get terminal symbol set
//std::set< std::string > MidCodeParser::get_termi()
//{
//    return termi_set;
//}
