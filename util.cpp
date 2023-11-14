#include"util.h"
#include "LexicalAnalyzer.h"
#include <sstream>

extern std::string Delimiter[DELIMITER_NUM];
extern std::string Operator[OPERATOR_NUM];

//读取文件为字符串
std::string readFileToString(const std::string& filePath) {
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        throw std::runtime_error("无法打开文件: " + filePath);
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    fileStream.close();
    return buffer.str();
}

// 判断字符是否为字母或下划线
bool isAlphaOrUnderscore(char letter)
{
	return((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z') || letter == '_') ? true : false;
}


// 判断字符是否为数字
bool isNumericDigit(char digit)
{
	return (digit >= '0' && digit <= '9') ? true : false;
}


// 判断字符是否为分隔符
bool isDelimiterChar(char ch)
{
	for (int i = 0; i < DELIMITER_NUM; i++) {
		if (Delimiter[i][0] == ch) {
			return true;
		}
	}
	return false;
}

// 判断字符是否为运算符
bool isOperatorChar(char ch)
{
	for (int i = 0; i < OPERATOR_NUM; i++) {
		if (Operator[i][0] == ch) {
			return true;
		}
	}
	return false;
}

// 将结果代码转换为对应的标志
int convertResultToTokenSign(int result)//用0-4转移到100-500
{
	int sign;
	switch (result) {
	case RESERVED_WORD:
		sign = RESERVED_WORD_SIGN;
		break;
	case IDENTIFIER:
		sign = IDENTIFIER_SIGN;
		break;
	case OPERATOR:
		sign = OPERATOR_SIGN;
		break;
	case DELIMITER:
		sign = DELIMITER_SIGN;
		break;
	case DIGIT:
		sign = DIGIT_SIGN;
		break;
	default:
		sign = DELIMITER_SIGN;
		break;
	}
	return sign;
}

// 根据标志转换为输出格式
void convertTokenSignToOutput(int sign, std::string& in_sign, std::string& output)
{
	if (sign == RESERVED_WORD_SIGN) {
		output = in_sign;
	}
	else if (sign == IDENTIFIER_SIGN) {
		output = "identifier";
	}
	else if (sign == OPERATOR_SIGN) {
		output = in_sign;
	}
	else if (sign == DELIMITER_SIGN) {
		output = in_sign;
	}
	else {
		output = "number";
	}
}




