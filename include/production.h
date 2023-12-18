#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <string>
#include <vector>

class Production
{
public:
    //Production();
    Production()
    {
        left="";
    }

    Production(std::string left, std::vector<std::string>right)
    {
        this->left = left;
        this->right = right;
    }

    std::string get_left() const;
    std::vector<std::string> get_right() const;
    void set_left(std::string left);
    void set_right(std::vector<std::string> right);
    void add_right(std::string rightOne);

private:
    std::string left;
    std::vector<std::string> right;
};

#endif // PRODUCTION_H
