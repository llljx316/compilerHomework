#pragma once
#include<iostream>
#include "common.h"
#include "util.h"
#include "abstractdfa.h"
using namespace std;

// ���
class DelimiterDetector : public AbstractDFA
{
private:

public:
    int findDelimiter(std::string ch);//�ж��Ƿ��ڽ������
    virtual int isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) override;
};


// ������
class KeywordDetector : public AbstractDFA
{
private:
    std::map<std::string, int> ReservedWord_Map;

public:
    KeywordDetector();
    virtual int isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) override;//override virtual function
};


// ����
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
    void resetState();//�ص���ʼ״̬
    int isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) override;
};


// ������
class OperatorDetector : public AbstractDFA
{
private:
    std::map<std::string, int> Operator_Map;
public:
    OperatorDetector();
    virtual int isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) override;
};


// ��ʶ��
class SymbolRecognizer : public AbstractDFA
{
private:
    uint cur_map_index = 0;//Token_Map�м���

public:
    void addToken(std::string ch);
    virtual int isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) override;
};