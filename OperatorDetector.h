﻿#ifndef OPERATORANALYZER_H
#define OPERATORANALYZER_H

#include"common.h"
#include"abstractdfa.h"

class OperatorDetector: public AbstractDFA
{
private:
     std::map<std::string, int> Operator_Map;
public:
    OperatorDetector();
    virtual int isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col) override;
};


#endif // OPERATORANALYZER_H
