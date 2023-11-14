#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <string>
#include "common.h"
#include <vector>
#include "abstractdfa.h"

#include "SymbolRecognizer.h"
#include "DelimiterDetector.h"
#include "OperatorDetector.h"
#include "KeywordDetector.h"
#include "NumericSequenceValidator.h"

#include "preprocessor.h"

class LexicalAnalyzer
{
private:
    Preprocessor preprocessor;
    AbstractDFA* DFAlist[5];
    static const int MAX_DIGIT_NUM=500;
    static const int MAX_IDENTIFIER_NUM=500;

public:
    LexicalAnalyzer();
    ~LexicalAnalyzer();
    int analyze(std::string &in_str,std::string &out_str, int& pre_line, int& pre_col);

    token digit_table[MAX_DIGIT_NUM];
    token identifier_table[MAX_IDENTIFIER_NUM];
    unsigned int line=0; //行数
    unsigned int start=0,end=0;//出错起始位置
};




#endif // LEXICALANALYZER_H
