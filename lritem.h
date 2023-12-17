#ifndef LRITEM_H
#define LRITEM_H

#include <vector>
#include <string>
#include <iostream>

class LR0
{
private:
    std::pair<unsigned int, unsigned int> item;
public:
    LR0()
    {
        item = std::make_pair(0,0);
    }
    LR0(unsigned int l, unsigned int r)
    {
        item = std::make_pair(l,r);
    }
    LR0(std::pair<unsigned int, unsigned int> item)
    {
        this->item = item;
    }

    std::pair<unsigned int, unsigned int> get_item() const;
    unsigned int get_left() const;
    unsigned int get_right() const;
    bool operator==(const LR0 &item) const;
    bool operator<(const LR0 &item) const;
};

class LR1
{
private:
    std::pair<LR0, std::string> item;
public:
    LR1()
    {
        item = std::make_pair(LR0(), "$");
    }
    LR1(const LR0 &first, const std::string &second)
    {
        item = std::make_pair(first, second);
    }
    LR1(std::pair<LR0, std::string> item)
    {
        this->item = item;
    }

    std::pair<LR0, std::string> get_item() const;
    LR0 get_left() const;
    std::string get_right() const;
    bool operator==(const LR1 &item) const;
    bool operator<(const LR1 &item) const;
};

#endif // LRITEM_H
