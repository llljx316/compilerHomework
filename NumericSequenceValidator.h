#ifndef DIGITANALYZER_H
#define DIGITANALYZER_H
#include<iostream>
#include "abstractdfa.h"
using namespace std;

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
    int isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col) override;
};

#endif // DIGITANALYZER_H
