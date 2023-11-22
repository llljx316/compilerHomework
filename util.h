#ifndef UTIL_H
#define UTIL_H
const unsigned int WIDTH=1200;
const unsigned int HEIGHT=900;
#include<iostream>
#include"common.h"
#include "abstractdfa.h"
#include <string>

bool isAlphaOrUnderscore(char letter);

bool isNumericDigit(char digit);
bool isDelimiterChar(char ch);
bool isOperatorChar(char ch);

int convertResultToTokenSign(int result);
void convertTokenSignToOutput(int sign, std::string& in_sign, std::string& output);

std::string readFileToString(const std::string& filePath);


class Preprocessor : public AbstractDFA
{
public:
    int preprocess(std::string& in_str, std::string& out_str, int& line, int& col);
    virtual int isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) override;

};

#endif // UTIL_H
