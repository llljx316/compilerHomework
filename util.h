#ifndef UTIL_H
#define UTIL_H
const unsigned int WIDTH=1200;
const unsigned int HEIGHT=900;
#include<iostream>


bool isAlphaOrUnderscore(char letter);

bool isNumericDigit(char digit);
bool isDelimiterChar(char ch);
bool isOperatorChar(char ch);

int convertResultToTokenSign(int result);
void convertTokenSignToOutput(int sign, std::string& in_sign, std::string& output);

std::string readFileToString(const std::string& filePath);

#endif // UTIL_H
