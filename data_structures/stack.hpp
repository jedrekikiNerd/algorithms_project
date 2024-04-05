#ifndef STACK
#define STACK

#include "list.h"

template <typename Type>
class Stack {
private:
    SingleListH<Type> list; // We create stack based on single list

public:
    // Push first element
    void push(Type value) 
    {
        list.add_front(value);
    }

    // Get head element from top
    Type pop() 
    {
        if (empty()) 
        {
            return Type();
        }
        Type top_value = list.first_value();
        list.remove_front();
        return top_value;
    }

    // Check if empty
    bool empty()
    {
        return list.get_size() == 0;
    }
};

#endif