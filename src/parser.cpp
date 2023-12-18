#include "parser.h"
#include "production.h"
#include <vector>
#include <string>
#include <fstream>
#include <fstream>
#include <cctype>
#include <queue>
#include <stack>
#include <sstream>
//#define test

#include <iostream>
using namespace std;

std::stack<std::string> str_stack;
std::stack<std::string> var_stack;
int temp_num;
int label_num;

std::string itoTemp(int i)
{
    std::ostringstream temp_output;
    temp_output << i << "_TEMP";
    return temp_output.str();
}

std::string itoLabel(int i)
{
    std::ostringstream temp_output;
    temp_output << i << "_LABEL";
    return temp_output.str();
}

std::string varStackPop()
{
    auto top_var = var_stack.top();
    var_stack.pop();
    return top_var;
}

std::string strStackPop()
{
    auto top_str = str_stack.top();
    str_stack.pop();
    return top_str;
}

std::string cmd1(std::string a, std::string b, std::string c, std::string d)
{
    return  "(" + a + ", " + b + ", " + c + ", " + d + ")";
}

//int Parser::translate(int id, std::string name)
//{
//#ifdef test
//    cout << "[translate] " << id << "  varSize=[" << var_stack.size() << "]";
//    cout << "    strSize=[" << str_stack.size() << "]" << endl;
//#endif // test
//    switch (id)
//    {
//    case 3:
//    {
//        varStackPop();
//        varStackPop();
//#ifdef test
//        cout << "[case 3] [var pop]" << endl;
//        cout << "[case 3] [var pop]" << endl;
//#endif // test
//        break;
//    }

//    case 9: case 10:
//    {
//        auto top_str = strStackPop();
//        std::ostringstream os;
//        os << std::endl << top_str;
//        str_stack.push(os.str());
//        break;
//    }

//    case 13:
//    {
//        varStackPop();
//#ifdef test
//        cout << "[case 13] [var pop]" << endl;
//#endif // test
//        break;
//    }

//    case 15:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        str_stack.push(os.str());
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        var_stack.push(l_var);
//#ifdef test
//        cout << "[case 15] [str pop] " << endl;
//        cout << "[case 15] [str pop] " << endl;
//        cout << "[case 15] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }

//    case 24:
//    {
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        std::ostringstream os;
//        os << strStackPop();
//        os << cmd1("=", r_var, "", l_var) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(l_var);
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 27:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << cmd1("or", l_str, r_var, itoTemp(temp_num)) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(itoTemp(temp_num));
//        temp_num++;
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 29:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << cmd1("and", l_var, r_var, itoTemp(temp_num)) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(itoTemp(temp_num));
//        temp_num++;
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 31:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << cmd1("==", l_var, r_var, itoTemp(temp_num)) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(itoTemp(temp_num));
//        temp_num++;
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 32:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << cmd1("!=", l_var, r_var, itoTemp(temp_num)) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(itoTemp(temp_num));
//        temp_num++;
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 34:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << cmd1("<", l_var, r_var, itoTemp(temp_num)) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(itoTemp(temp_num));
//        temp_num++;
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 35:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << cmd1(">", l_var, r_var, itoTemp(temp_num)) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(itoTemp(temp_num));
//        temp_num++;
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 36:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << cmd1("<=", l_var, r_var, itoTemp(temp_num)) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(itoTemp(temp_num));
//        temp_num++;
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 37:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << cmd1(">=", l_var, r_var, itoTemp(temp_num)) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(itoTemp(temp_num));
//        temp_num++;
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 39:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << cmd1("+", l_var, r_var, itoTemp(temp_num)) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(itoTemp(temp_num));
//        temp_num++;
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 40:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << cmd1("-", l_var, r_var, itoTemp(temp_num)) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(itoTemp(temp_num));
//        temp_num++;
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 42:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << cmd1("*", l_var, r_var, itoTemp(temp_num)) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(itoTemp(temp_num));
//        temp_num++;
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 43:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        os << l_str << r_str;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << cmd1("/", l_var, r_var, itoTemp(temp_num)) << std::endl;
//        str_stack.push(os.str());
//        var_stack.push(itoTemp(temp_num));
//        temp_num++;
//#ifdef test
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str pop] " << endl;
//        cout << "[case" << id << "] [str push] " << os.str() << endl;
//#endif // test
//        break;
//    }
//    case 44: case 45: case 46: case 47:
//    {
//        str_stack.push("");
//#ifdef test
//        cout << "[case" << id << "] [str push] []" << endl;
//#endif // test
//        break;
//    }

//    case 49:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();

//        os << cmd1(itoLabel(label_num), "", "", "") << std::endl;
//        os << l_str;
//        os << cmd1("J!=", l_var, "0", itoLabel(label_num + 1)) << std::endl;
//        os << cmd1("J", "", "", itoLabel(label_num + 2)) << std::endl;
//        os << cmd1(itoLabel(label_num + 1), "", "", "") << std::endl;
//        os << r_str;
//        os << cmd1("J", "", "", itoLabel(label_num)) << std::endl;
//        os << cmd1(itoLabel(label_num + 2), "", "", "") << std::endl;
//        label_num += 3;
//        str_stack.push(os.str());
//        var_stack.push(l_var);
//        break;
//    }

//    case 52:
//    {
//        auto r_str = strStackPop();
//        auto l_str = strStackPop();
//        std::ostringstream os;
//        auto r_var = varStackPop();
//        auto l_var = varStackPop();
//        os << l_str;
//        os << cmd1("J!=", l_var, "0", itoLabel(label_num)) << std::endl;
//        os << cmd1("J", "", "", itoLabel(label_num + 1)) << std::endl;
//        os << cmd1(itoLabel(label_num), "", "", "") << std::endl;;
//        os << r_str;
//        os << cmd1(itoLabel(label_num + 1), "", "", "") << std::endl;
//        label_num += 2;
//        str_stack.push(os.str());
//        var_stack.push(l_var);
//        break;
//    }

//    case 53:
//    {
//        auto r_str = strStackPop();
//        auto m_str = strStackPop();
//        auto l_str = strStackPop();

//        std::ostringstream os;
//        auto r_var = varStackPop();
//        auto m_var = varStackPop();
//        auto l_var = varStackPop();

//        os << l_str;
//        os << cmd1("J!=", l_var, "0", itoLabel(label_num)) << std::endl;
//        os << cmd1("J", "", "", itoLabel(label_num + 1)) << std::endl;
//        os << cmd1(itoLabel(label_num), "", "", "") << std::endl;;
//        os << m_str;
//        os << cmd1("J", "", "", itoLabel(label_num + 2)) << std::endl;
//        os << cmd1(itoLabel(label_num + 1), "", "", "") << std::endl;
//        os << r_str;
//        os << cmd1(itoLabel(label_num + 2), "", "", "") << std::endl;
//        label_num += 3;
//        str_stack.push(os.str());
//        var_stack.push(l_var);
//        break;
//    }

//    default:
//        break;
//    }
//#ifdef test
//    cout << "[/translate] " << id << "  varSize=[" << var_stack.size() << "]";
//    cout << "    strSize=[" << str_stack.size() << "]" << endl;
//    cout << endl;
//#endif // test
//    return 1;
//}

// analyze the tokens with grammar
int Parser::analyse(const std::vector<Token>& tokens)
{
    temp_num = 0;
    label_num = 0;
    build();

    //输出文件重定向
    ofstream fout;
    fout.open("SysStack.txt", ios::out);
    if (fout.is_open() == 0)
    {
        cout << "output open failed" << endl;
        exit(-1);
    }

    std::cout << std::endl;
    std::cout << "Symbol stack information:" << std::endl;

    //在此输出到文件
    fout << "Symbol stack information:" << std::endl;

    //system("pause");

    std::stack<std::pair<unsigned int, std::string>> the_stack;
    the_stack.push({ 0,"$" });
    auto token_iter = tokens.cbegin();
    //在这里输出程序运行中插入token的信息
    std::cout << "Inserted element:" << std::endl;
    std::cout << token_iter->get_name() << " " << token_iter->get_type_output() << std::endl;

    //在此输出到文件
    fout << "Inserted element:" << std::endl;
    fout << token_iter->get_name() << " " << token_iter->get_type_output() << std::endl;
    for (; ; )
    {
        auto st_top_first = the_stack.top().first;
        std::string the_type;
        if (token_iter->get_type() == TokenType::ID || token_iter->get_type() == TokenType::CHAR
                || token_iter->get_type() == TokenType::INT || token_iter->get_type() == TokenType::FLOAT)
            the_type = TokenDict[token_iter->get_type()];
        else
            the_type = token_iter->get_name();

        if (action[st_top_first].find(the_type) != action[st_top_first].end())
        {
            auto the_action = action[st_top_first][the_type];
            if (the_action.first == "S")
            {
                if (token_iter->get_type() == TokenType::ID || token_iter->get_type() == TokenType::CHAR
                        || token_iter->get_type() == TokenType::INT || token_iter->get_type() == TokenType::FLOAT)
                {
                    var_stack.push(token_iter->get_name());
                    //cout<<"[pushVAR] "<<token_iter->get_name() <<"   [at line "<<token_iter->get_line()<<"]"<<endl;
                }
                the_stack.push({ the_action.second, the_type });

                //在这里输出程序运行中栈的信息
                std::cout << "Stack Information:" << std::endl;
                for (std::stack<std::pair<unsigned int, std::string>> dump = the_stack; !dump.empty(); dump.pop())
                    std::cout << dump.top().first << " " << dump.top().second << '\n';
                std::cout << std::endl;

                //在此输出到文件
                fout << "Stack Information:" << std::endl;
                for (std::stack<std::pair<unsigned int, std::string>> dump = the_stack; !dump.empty(); dump.pop())
                    fout << dump.top().first << " " << dump.top().second << '\n';
                fout << std::endl;

                //system("pause");

                token_iter++;
                //在这里输出程序运行中插入token的信息
                std::cout << "Inserted element:" << std::endl;
                std::cout << token_iter->get_name() << " " << token_iter->get_type_output() << std::endl;

                //在此输出到文件
                fout << "Inserted element:" << std::endl;
                fout << token_iter->get_name() << " " << token_iter->get_type_output() << std::endl;
            }
            else if (the_action.first == "r")
            {
                auto id = the_action.second;
                auto the_right = grammar[id].get_right();
                for (unsigned int i = 0; i < the_right.size(); i++)
                    if (the_right[i] != "@")
                        the_stack.pop();
                auto new_top_first = the_stack.top().first;
                the_stack.push({ go[new_top_first][grammar[id].get_left()],grammar[id].get_left() });

                // translate
#ifdef test
                //cout<<"use production["<<id<<"]: "<<grammar[id].get_left()<<"->";
                //for(auto e:the_right)cout<<e<<" ";
                //cout<<endl;
#endif // test
//                if (!translate(id, token_iter->get_name()))
//                {
//                    std::cout << "ERROR! at line " << token_iter->get_line() << std::endl;
//                    return token_iter->get_line();
//                }

                /*
                //在这里输出程序运行中插入token的信息
                std::cout << "插入的元素：" << std::endl;
                std::cout << token_iter->get_name() << " " << token_iter->get_type_output() << std::endl;
                //在这里输出程序运行中栈的信息
                std::cout << "栈中信息：" << std::endl;
                for (std::stack<std::pair<unsigned int, std::string>> dump = the_stack; !dump.empty(); dump.pop())
                    std::cout << dump.top().first << " " << dump.top().second << '\n';
                system("pause");
                */

            }

            else if (the_action.first == "acc")
            {
                //关闭SysStack.txt文件
                fout.close();

                ofstream fout;
                fout.open("Quaternion_information.txt", ios::out);
                if (fout.is_open() == 0)
                {
                    cout << "output open failed" << endl;
                    exit(-1);
                }

                std::cout << "Accept!" << std::endl;
                //输出到文件
                fout << "Accept!" << std::endl;

//                //在这里输出最后的四元式
//                std::cout << "Quaternion information:" << std::endl;
//                std::cout << str_stack.top() << std::endl;
//                //输出到文件
//                fout << "Quaternion information:" << std::endl;
//                fout << str_stack.top() << std::endl;
                fout.close();
                return 0;
            }
        }

        else
        {
            std::cout << "ERROR! at line " << token_iter->get_line() << std::endl;
            return token_iter->get_line();
        }
    }
    return 0;
}

// get one symbol's first set
std::set<std::string> Parser::get_first(const std::string& symbol)
{
    if (first_map.find(symbol) != first_map.end())         // have this set, then return
        return first_map[symbol];
    std::set<std::string> end_ret;
    if (termi_set.find(symbol) != termi_set.end())   // is a tmp_termi symbol
    {
        end_ret.insert(symbol);
        return end_ret;
    }
    else                                                // is a variable symbol
    {
        for (auto tmp_pro : grammar)
        {
            if (tmp_pro.get_left() == symbol)
            {
                auto the_right = tmp_pro.get_right();
                if (the_right.size() == 1 && the_right[0] == "@")  //insert @
                {
                    end_ret.insert("@");
                    continue;
                }
                else if (the_right.size() > 0                //insert the first tmp_termi symbol
                         && termi_set.find(the_right[0]) != termi_set.end())
                {
                    end_ret.insert(the_right[0]);
                    continue;
                }
            }
        }
    }
    return end_ret;
}

// build the first Map, first_map[x] = first(x)
void Parser::gen_first_map()
{
    first_map.clear();

    // basic
    for (auto tmp_symbol : termi_set)
        first_map.insert({ tmp_symbol, get_first(tmp_symbol) });
    for (auto tmp_symbol : var_set)
        first_map.insert({ tmp_symbol, get_first(tmp_symbol) });

    // merge
    for (; ; )
    {
        bool changed = false;
        for (auto tmp_symbol : var_set)
        {
            //if(symbol == "ADD")cout<<"get first (add) "<<endl;
            for (;;)
            {
                auto tmp_first = first_map[tmp_symbol];
                //if(symbol == "ADD"){cout<<"first= ";for(auto e:temp)cout<<e<<" ";cout<<endl;}
                for (auto tmp_pro : grammar)
                {
                    if (tmp_pro.get_left() == tmp_symbol)
                    {
                        bool all_empty = true;
                        auto the_right = tmp_pro.get_right();
                        //if(symbol == "ADD"){cout<<"  the_right= ";for(auto e:the_right)cout<<e<<" ";cout<<endl;}
                        for (auto tmp_right : the_right)
                        {
                            auto tmp_right_set = first_map[tmp_right];
                            //if(symbol == "ADD"){cout<<"  oneset= ";for(auto e:oneSet)cout<<e<<" ";cout<<endl;}
                            if (tmp_right_set.erase("@"))
                            {
                                tmp_first.insert(tmp_right_set.begin(), tmp_right_set.end());
                            }
                            else
                            {
                                tmp_first.insert(tmp_right_set.begin(), tmp_right_set.end());
                                all_empty = false;
                                break;
                            }
                        }
                        if (all_empty)
                            tmp_first.insert("@");
                    }
                }
                //if(symbol == "ADD"){cout<<" now tot = ";for(auto e:temp)cout<<e<<" ";cout<<endl;}
                if (tmp_first == first_map[tmp_symbol])
                    break;
                first_map[tmp_symbol] = tmp_first;
                changed = true;
            }
        }
        if (!changed)
            break;
    }
}

// get first(beta), beta = A1A2...An
std::set<std::string> Parser::get_first(const std::vector<std::string>& beta)
{
    std::set<std::string> end_ret;
    bool all_empty = true;
    for (auto tmp_beta : beta)
    {
        auto tmp_beta_set = first_map[tmp_beta];
        if (tmp_beta_set.erase("@"))
        {
            end_ret.insert(tmp_beta_set.begin(), tmp_beta_set.end());
        }
        else
        {
            end_ret.insert(tmp_beta_set.begin(), tmp_beta_set.end());
            all_empty = false;
            break;
        }
    }
    if (all_empty)
        end_ret.insert("@");
    return end_ret;
}

// build action and goto table
void Parser::build()
{
    if (closure_map.size() != 0)                      // have built action and goto table
        return;

    gen_first_map();

    auto closure0 = get_closure(LR1(LR0(0, 0), "$"));

    std::queue<LR1set> LR1_queue;
    LR1_queue.push(closure0);

    //closure_map[closure0] = closure_map.size();     // first insert closure, then map[closure] = 1

    closure_list.push_back(closure0);
    closure_map.insert(make_pair(closure0, closure_map.size()));


    while (!LR1_queue.empty())
    {
        auto tmp_LR1_set = LR1_queue.front();
        LR1_queue.pop();
        transfer.push_back(vpsi());

        for (auto variable : var_set)
        {
            LR1set newset;
            for (auto tmp_LR1 : tmp_LR1_set)
            {
                auto tmp_LR0 = tmp_LR1.get_left();
                auto id = tmp_LR0.get_left();
                auto pt_pos = tmp_LR0.get_right();
                if (grammar[id].get_right().size() > pt_pos && grammar[id].get_right()[pt_pos] == variable)
                    newset.insert(LR1(LR0(id, pt_pos + 1), tmp_LR1.get_right()));
            }
            if (newset.size() != 0)
            {
                get_closure(newset);
                if (closure_map.find(newset) == closure_map.end())
                {
                    closure_list.push_back(newset);
                    closure_map.insert(make_pair(newset, closure_map.size()));
                    //closure_map[newset]=closure_map.size();
                    LR1_queue.push(newset);
                }
                transfer[transfer.size() - 1].push_back(make_pair(variable, closure_map[newset]));
            }
        }

        for (auto tmp_termi : termi_set)
        {
            LR1set newset;
            for (auto tmp_LR1 : tmp_LR1_set)
            {
                auto tmp_LR0 = tmp_LR1.get_left();
                auto id = tmp_LR0.get_left();
                auto pt_pos = tmp_LR0.get_right();
                if (grammar[id].get_right().size() != pt_pos && grammar[id].get_right()[pt_pos] == tmp_termi)
                    newset.insert(LR1(LR0(id, pt_pos + 1), tmp_LR1.get_right()));
            }
            if (newset.size() != 0)
            {
                get_closure(newset);
                if (closure_map.find(newset) == closure_map.end())
                {
                    closure_list.push_back(newset);
                    closure_map.insert(make_pair(newset, closure_map.size()));
                    //closure_map[newset]=closure_map.size();
                    LR1_queue.push(newset);
                }
                transfer[transfer.size() - 1].push_back(std::make_pair(tmp_termi, closure_map[newset]));
            }
        }
    }

    for (unsigned int i = 0; i < closure_list.size(); i++)
    {
        action.push_back(std::map<std::string, psi>());
        go.push_back(std::map<std::string, int>());

        for (auto tmp_LR1 : closure_list[i])
        {
            auto tmp_pt = tmp_LR1.get_left().get_right();
            auto the_right = grammar[tmp_LR1.get_left().get_left()].get_right();
            if (tmp_pt < the_right.size() && termi_set.find(the_right[tmp_pt]) != termi_set.end())
            {
                for (auto go : transfer[i])
                    if (go.first == the_right[tmp_pt])
                    {
#ifdef test
                        if (action[i].find(the_right[tmp_pt]) != action[i].end()
                                && action[i][the_right[tmp_pt]] != pair<string, int>("S", go.second))
                        {
                            cout << "error 1 at action[" << i << "][" << the_right[tmp_pt] << "]=S" << go.second;
                            cout << "  old=" << action[i][the_right[tmp_pt]].first << action[i][the_right[tmp_pt]].second << endl;
                        }
#endif // test
                        action[i][the_right[tmp_pt]] = std::make_pair("S", go.second);
                    }
            }
        }
        for (auto tf : transfer[i])
        {
            if (var_set.find(tf.first) != var_set.end())
            {
#ifdef test
                if (go[i].find(tf.first) != go[i].end())
                    cout << "error at go[" << i << "][" << tf.first << "]" << endl;
#endif // test
                go[i][tf.first] = tf.second;

            }
        }
        for (auto tmp_LR1 : closure_list[i])
        {
            auto pre_next = tmp_LR1.get_right();
            auto tmp_LR0 = tmp_LR1.get_left();
            auto id = tmp_LR0.get_left();
            auto tmp_pt = tmp_LR0.get_right();
            auto the_right = grammar[id].get_right();

            if (tmp_pt == the_right.size())
            {
#ifdef test
                if (action[i].find(pre_next) != action[i].end()
                        && action[i][pre_next] != pair<string, int>("r", id))
                    cout << "error 2 at action[" << i << "][" << pre_next << "]=r" << id << " old=" << action[i][pre_next].first << action[i][pre_next].second << std::endl;
#endif
                // deal with if else / if
                if (!(action[i].find(pre_next) != action[i].end()
                      && action[i][pre_next] != std::pair<std::string, int>("r", id)))
                    action[i][pre_next] = std::make_pair("r", id);

                if (pre_next == "$" && id == 0)
                {
                    action[i][pre_next] = std::make_pair("acc", 0);
                }
            }
        }
    }
}

// open grammar.txt, read grammar
bool Parser::open_file(const std::string& fileName)
{
    std::ifstream in(fileName);

    if (in)
    {
        std::string str;
        int the_type = 0;
        while (in >> str)
        {
            if (str == "[terminal]")
            {
                the_type = 1;
                continue;
            }
            else if (str == "[variable]")
            {
                the_type = 2;
                continue;
            }
            else if (str == "[production]")
                break;
            else
            {
                if (the_type == 1)
                    termi_set.insert(str);
                else if (the_type == 2)
                    var_set.insert(str);
            }
        }

        while (getline(in, str))
        {
            if (str.empty())
                continue;
            std::string temp;
            std::istringstream iss = std::istringstream(str);
            Production production;
            iss >> temp;
            iss >> temp;
            production.set_left(temp);
            while (iss >> temp)
            {
                if (temp == "->")
                    continue;
                production.add_right(temp);
            }
#ifdef test
            /*
            cout<<production.get_left()<<"->";
            for(auto e:production.get_right())
              cout<<e<<" ";
            cout<<endl;
            */
#endif // test
            grammar.push_back(production);
        }
        return true;
    }
    else
    {
        std::cerr << fileName << "开启失败" << std::endl;
        return false;
    }
}

// get grammar
std::vector<Production> Parser::read_grammer()
{
    return grammar;
}

void Parser::get_closure(LR1set& closure)
{
    std::set<LR1> temp;

    for (;;)
    {
        temp = closure;

        for (auto tmp_LR1 : closure)
        {
            LR0 tmp_LR0 = tmp_LR1.get_left();
            std::string pre_next = tmp_LR1.get_right();

            unsigned int pt_pos = tmp_LR0.get_right();
            auto the_right = grammar[tmp_LR0.get_left()].get_right();

            if (pt_pos != the_right.size() && var_set.find(the_right[pt_pos]) != var_set.end())
            {
                std::string B = the_right[pt_pos];

                std::vector<std::string> beta;
                for (unsigned int i = pt_pos + 1; i < the_right.size(); i++)
                    beta.push_back(the_right[i]);
                beta.push_back(pre_next);

                auto first = get_first(beta);

                for (unsigned int pro_id = 0; pro_id < grammar.size(); pro_id++)
                {
                    const Production& production = grammar[pro_id];
                    if (production.get_left() == B)
                    {
                        for (auto tmp_termi : first)
                        {
                            LR1 new_item;
                            if (production.get_right().size() == 1 && production.get_right()[0] == "@")
                                new_item = LR1(LR0(pro_id, 1), tmp_termi);
                            else
                                new_item = LR1(LR0(pro_id, 0), tmp_termi);
                            if (closure.find(new_item) == closure.end())
                                temp.insert(new_item);
                        }
                    }
                }
            }
        }
        if (temp == closure)
            break;
        else
            closure = temp;
    }
}
// get a closure set with LR1 item
std::set<LR1> Parser::get_closure(const LR1& item)
{
    std::set<LR1> closure = { item };
    get_closure(closure);
    return closure;
}

// get the closure


// get closure list
std::vector< std::set<LR1> > Parser::get_closure()
{
    return closure_list;
}

// get closure map
std::map< std::set<LR1>, int> Parser::get_closure_map()
{
    return closure_map;
}

// get transfer
std::vector< vpsi > Parser::get_transfer()
{
    return transfer;
}

// get action table
std::vector< std::map<std::string, psi>> Parser::get_action()
{
    return action;
}

// get goto table
std::vector< std::map<std::string, int>> Parser::get_go()
{
    return go;
}

// get variable symbol set
std::set< std::string > Parser::get_var()
{
    return var_set;
}

// get tmp_termi symbol set
std::set< std::string > Parser::get_termi()
{
    return termi_set;
}
