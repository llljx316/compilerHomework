#include "production.h"

std::string Production::get_left() const
{
    return left;
}

std::vector<std::string> Production::get_right() const
{
    return right;
}

void Production::set_left(std::string left)
{
    this->left = left;
}

void Production::set_right(std::vector<std::string> right)
{
    this->right = right;
}

void Production::add_right(std::string rightOne)
{
    right.push_back(rightOne);
}
