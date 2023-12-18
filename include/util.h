#ifndef UTIL_H
#define UTIL_H
const unsigned int WIDTH=1200;
const unsigned int HEIGHT=900;
#include<iostream>
#include <string>

using uint=unsigned int;

const int CANNOT_ENTER=-2;
const int NOT=-1;
const int SUCCESS=1;
const int PreProcess_NOT=-50;
const int NOT_BOUND=1000;//原来不在表中
const int RIGHT_STATUS=10000;

const int RESERVED_WORD=0;
const int IDENTIFIER=1;
const int OPERATOR=2;
const int DELIMITER=3;
const int DIGIT=4;

const int RESERVED_WORD_SIGN=100;
const int IDENTIFIER_SIGN=200;
const int OPERATOR_SIGN=300;
const int DELIMITER_SIGN=400;
const int DIGIT_SIGN=500;



bool isAlphaOrUnderscore(char letter);

bool isNumericDigit(char digit);
bool isDelimiterChar(char ch);
bool isOperatorChar(char ch);

int convertResultToTokenSign(int result);
void convertTokenSignToOutput(int sign, std::string& in_sign, std::string& output);

std::string readFileToString(const std::string& filePath);
void Preprocess(const std::string& filename);

#endif // UTIL_H
