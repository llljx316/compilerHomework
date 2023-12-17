#include "lritem.h"

std::pair<unsigned int, unsigned int> LR0::get_item() const
{
    return item;
}

unsigned int LR0::get_left() const
{
    return item.first;
}

unsigned int LR0::get_right() const
{
    return item.second;
}

bool LR0::operator==(const LR0& item) const
{
    return this->get_item() == item.get_item();
}

bool LR0::operator<(const LR0& item) const
{
    return this->get_item() < item.get_item();
}



std::pair<LR0, std::string> LR1::get_item() const
{
    return item;
}

LR0 LR1::get_left() const
{
    return item.first;
}

std::string LR1::get_right() const
{
    return item.second;
}

bool LR1::operator==(const LR1& item) const
{
    return this->get_item() == item.get_item();
}


bool LR1::operator<(const LR1& item) const
{
    return this->get_item() < item.get_item();
}
