#ifndef TO_STRINGS
#define TO_STRINGS

#include <iostream>

// Depending in type choose
template <typename Type>
std::string choose_to_string(const Type& value)
{
    if constexpr (std::is_same_v<Type, int>)
    {
        return std::to_string(value);
    }
    else if constexpr (std::is_same_v<Type, std::string>)
    {
        return std::to_string(value);
    }
    else
    {
        return "***";
    }
}

#endif