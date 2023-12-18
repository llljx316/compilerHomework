#include "token.h"

void Token::set_name(const std::string& name)
{
    this->name = name;
}

std::string Token::get_name() const
{
    return this->name;
}

void Token::set_type(TokenType type)
{
    this->type = type;
}

TokenType Token::get_type() const
{
    return this->type;
}

void Token::set_line(size_t line)
{
    this->line = line;
}

size_t Token::get_line() const
{
    return this->line;
}

std::string Token::get_type_output() const
{
    if (this->type == TokenType::KEYWORD)
        return "KEYWORD";
    if (this->type == TokenType::ID)
        return "ID";
    if (this->type == TokenType::INT)
        return "INT";
    if (this->type == TokenType::FLOAT)
        return "FLOAT";
    if (this->type == TokenType::CHAR)
        return "CHAR";
    if (this->type == TokenType::OPERATOR)
        return "OPERATOR";
    if (this->type == TokenType::DELIMITER)
        return "DELIMITER";
    if (this->type == TokenType::OTHER)
        return "OTHER";
    return nullptr;
}
