#pragma once
#include<iostream>
#include "common.h"
#include "util.h"
#include "abstractdfa.h"
using namespace std;

// 界符
class DelimiterDetector : public AbstractDFA
{
private:

public:
    int findDelimiter(std::string ch);//判断是否在界符表里
    virtual int isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) override;
};


// 保留字
class KeywordDetector : public AbstractDFA
{
private:
    std::map<std::string, int> ReservedWord_Map;

public:
    KeywordDetector();
    virtual int isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) override;//override virtual function
};


// 数字
class NumericSequenceValidator : public AbstractDFA
{
private:
    enum STATE
    {
        STATE_START = 0,
        STATE_1 = 1,
        STATE_2 = 2,
        STATE_3 = 3,
        STATE_4 = 4,
        STATE_ERROR = 5,
        STATE_TERMINATE = 6
    };
    int state;
public:
    NumericSequenceValidator();
    void resetState();//回到初始状态
    int isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) override;
};


// 操作符
class OperatorDetector : public AbstractDFA
{
private:
    std::map<std::string, int> Operator_Map;
public:
    OperatorDetector();
    virtual int isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) override;
};


// 标识符
class SymbolRecognizer : public AbstractDFA
{
private:
    uint cur_map_index = 0;//Token_Map有几个

public:
    void addToken(std::string ch);
    virtual int isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) override;
};