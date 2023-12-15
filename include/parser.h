#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <stack>


const bool NON_TERMINATOR = 1;
const bool TERMINATOR = 0;
const int ACC = -10086;
#define SYNTAX_SUCCESS -1

#define LINEOFNONT -2

struct Action
{
    std::string symbol_name; // 符号名称
    int dst; // 目标状态
    Action* next = nullptr; // 下一个动作项

    // 默认构造函数，初始化为"head"符号和目标状态为-1
    Action() : symbol_name("head"), dst(-1) {}

    // 带参数的构造函数，允许设置符号名称和目标状态
    Action(const std::string& s, const int d) : symbol_name(s), dst(d) {}
};

// 表示状态之间的转移信息
struct StateTransition
{
    std::string symbol_name; // 符号名称，触发状态转移的输入符号
    int dst;  // 目标状态
    StateTransition* next = nullptr; // 下一个状态转移项

    // 默认构造函数，初始化为"head"符号和目标状态为-1
    StateTransition() : symbol_name("head"), dst(-1) {}

    // 带参数的构造函数，允许设置符号名称和目标状态
    StateTransition(const std::string& s, const int d) : symbol_name(s), dst(d) {}
};


// 表示语法分析步骤，包括动作和状态转移
struct ParseStep
{
    Action* action_ptr;       // 动作指针
    StateTransition* goto_ptr; // 状态转移指针

    // 默认构造函数，初始化动作指针和状态转移指针
    ParseStep()
    {
        action_ptr = new Action("head", -1);
        goto_ptr = new StateTransition("head", -1);
    }
};


// 表示用于 LR(1) 验证的条目
struct LR1VerificationEntry
{
    int index;          // 索引
    std::string name;   // 名称
    int dst;            // 目标

    // 比较操作符，用于排序
    bool operator < (const LR1VerificationEntry& a) const
    {
        if (index != a.index)
            return index < a.index;
        else
            return name < a.name;
    }

    // 构造函数，允许设置索引、名称和目标
    LR1VerificationEntry(int i, std::string n, int d) : index(i), name(n), dst(d)
    {}
};


// 表示符号信息
class InfoSymbol
{
public:
    std::string name;         // 符号名称
    bool type = 0;            // 符号类型（0表示终结符，1表示非终结符）
    int generators_index = -1; // 生成器索引

    // 比较操作符，用于排序
    bool operator <(const InfoSymbol& a) const
    {
        return this->name < a.name;
    }

    // 默认构造函数
    InfoSymbol() {}

    // 带参数的构造函数，允许设置符号名称、类型和生成器索引
    InfoSymbol(std::string n, bool t = 0, int i = -1) : name(n), type(t), generators_index(i) {}
};



// 表示生成规则
class ProductionRule
{
public:
    InfoSymbol left;              // 左部符号
    std::vector<InfoSymbol> right_list;  // 右部符号列表
    unsigned int order = 0;      // 规则顺序

    // 默认构造函数
    ProductionRule() {}

    // 带参数的构造函数，允许设置左部符号、右部符号列表和规则顺序
    ProductionRule(InfoSymbol l, std::vector<InfoSymbol> r_list, unsigned int o)
        : left(l), right_list(r_list), order(o)
    {}

    // 拷贝构造函数
    ProductionRule(const ProductionRule& a)
        : left(a.left), right_list(a.right_list), order(a.order)
    {}

    // 比较操作符，用于排序
    bool operator <(const ProductionRule& a) const
    {
        return this->order < a.order; // 相同顺序，相同内容
    }
};



// 表示项目，包括点和展望符号
class item
{
public:
    const ProductionRule base;       // 基本生成规则
    unsigned int index;              // 索引
    std::set<std::string> prospect_symbols; // 展望符号集合

    // 默认构造函数
    item(const ProductionRule& b, unsigned int i) : base(b), index(i) {}

    // 比较操作符，用于排序
    bool operator <(const item& a) const
    {
        if (this->base < a.base || a.base < this->base)
        {
            return this->base < a.base;
        }
        else if (this->index < a.index || this->index > a.index)
            return this->index < a.index;
        else
        {
            if (this->prospect_symbols.size() != a.prospect_symbols.size())
                return this->prospect_symbols.size() < a.prospect_symbols.size();

            if (this->prospect_symbols == a.prospect_symbols)
                return false;

            bool tag = false; // 保留标记变量
            auto ite1 = this->prospect_symbols.begin();
            auto ite2 = a.prospect_symbols.begin();

            for (; ite1 != this->prospect_symbols.end(); ite1++)
            {
                if (*ite1 < *ite2 || *ite2 < *ite1)
                    return *ite1 < *ite2;
                ite2++;
            }
            return tag;
        }
    }
};

// 表示项目集
class ItemSet
{
public:
    int id;                // 标识符
    std::set<item> items;  // 项目集中的项目

    // 默认构造函数
    ItemSet() {}

    // 带参数的构造函数，允许设置标识符
    ItemSet(int i) : id(i) {}

    // 比较操作符，用于排序
    bool operator <(const ItemSet& a) const
    {
        if (this->items.size() != a.items.size())
            return items.size() < a.items.size();
        std::set<item>::iterator ite = this->items.begin();

        bool tag = false;

        for (auto const& i : a.items)
        {
            if (*ite < i || i < *ite)
                return *ite < i;
            ite++;
        }
        return tag;
    }
};

// 表示词法单元
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
};

// 解析器类
class Parser
{
public:
    // 每个项目集遇到不同符号时的动作映射
    std::vector < std::map<std::string, int>> go_map;
    // 生成规则列表，按左部符号分组
    std::vector<std::vector<ProductionRule>> generators_list;
    // 项目状态栈
    std::stack<int> itemS;
    // 符号栈
    std::stack<Lexeme> symbolS;
    // 终结符集合
    std::set<InfoSymbol> terminators;
    // 所有生成规则的列表
    std::vector<ProductionRule> pure_generator_list;
    // 所有符号集合
    std::set<InfoSymbol> symbols;
    // 项目集集合
    std::set<ItemSet> item_groups;
    // 非终结符集合
    std::set<InfoSymbol> non_terminators;

    // 获取符号的First集合
    void get_symbol_first(const InfoSymbol& a);
    // 获取所有符号的First集合
    void get_all_symbol_first();
     // 打印DFA
    void print_DFA();
    // 检查输入文件的语法
    int check(const std::string path);
    // 计算项目集之间的状态转移
    void item_group_go(const ItemSet& scr, ItemSet& dst, std::string input);
    // 获取序列的First集合
    void get_sequence_first(const std::vector<std::string>& seq, std::vector<std::string>& re);
    // 计算项目集的闭包
    void get_closure(ItemSet& group);
    // 从文件中读取语法规则
    void read_grammer(const std::string path);
    // 构建项目集列表
    void get_item_group_list();
    // 构建LR(1)分析表
    void get_LR1_table();
    // 打印LR(1)分析表
    void print_LR1_table();

private:  
    // 符号到First集合的映射
    std::map<std::string, std::set<InfoSymbol>> symbol2first;
    // 分析表
    std::vector < std::pair<std::map<std::string, int>, std::map<std::string, int>>> table;
    // LR(1)分析表
    std::vector<ParseStep> LR1_table;

    // 终结符数量
    unsigned int num_termi = 0;
    // 非终结符数量
    unsigned int num_nontermi = 0;
};

#endif // PARSER_H
