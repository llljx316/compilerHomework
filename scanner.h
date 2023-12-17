#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"

#include <vector>

enum class Process
{
    BEGIN,
    IN_KEYWORD,
    IN_ID,
    IN_INTEGER,
    IN_FLOAT,
    IN_OPERATOR,
    IN_DELIMITER,
    IN_CHAR,
    END
};

class Scanner
{
public:
    Scanner();
    std::vector<Token> getTokens(const std::string file_name);
    std::string Preprocessing(std::string input_str, int Input_str_len);

private:
    Process proc;
    std::string file_name;
    std::string code;
    std::string::const_iterator iter;
    size_t line;


    void BeginDetector(std::string& name, TokenType& type);
    void KeyWordDetector(std::string& name, TokenType& type);
    void IDDetector(std::string& name, TokenType& type);
    void IntDetector(std::string& name, TokenType& type);
    void FloatDetector(std::string& name, TokenType& type);
    void OperatorDetector(std::string& name, TokenType& type);
    void DelimiterDetector(std::string& name, TokenType& type);
    //void dealString(std::string &name, TokenType &type);
    void CharDetector(std::string& name, TokenType& type);
    Token EndDetector(const std::string& name, const TokenType& type);

    void skip_blank();
    bool open_file(const std::string& file_name);
    Token get_next_token();

};

#endif // SCANNER_H
