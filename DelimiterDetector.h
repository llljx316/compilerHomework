#ifndef DELIMITERSANALYZER_H
#define DELIMITERSANALYZER_H

#include "common.h"
#include "abstractdfa.h"

class DelimiterDetector : public AbstractDFA
{
private:

public:
    int findDelimiter(std::string ch);//判断是否在界符表里
    virtual int isAccepted(const std::string &str, unsigned int &start, unsigned int &end, unsigned int &line, unsigned int& col) override;
};



#endif // DELIMITERSANALYZER_H
