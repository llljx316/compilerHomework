#ifndef ABSTRACTDFA_H
#define ABSTRACTDFA_H

#include "common.h"

class AbstractDFA
{
public:
    explicit AbstractDFA();
    virtual int isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line,unsigned int &col)=0;

};

#endif // ABSTRACTDFA_H
