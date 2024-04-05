#ifndef I_DATA
#define I_DATA
#include<iostream>
/**
 * DataStructure Interface
 * 
 */
template <typename Type>
class IDataStructure
{
public:
    ~IDataStructure() {}

    // Adds value on first index
    virtual void add_front(Type value) = 0;
    // Adds value on last index
    virtual void add_back(Type value) = 0;
    // Adds value on given index
    virtual void add_at(Type value, unsigned int position) = 0;

    // Removes value on first index
    virtual void remove_front() = 0;
    // Removes value on last index
    virtual void remove_back() = 0;
    // Removes value on given index
    virtual void remove_at(unsigned int position) = 0;
    // Clears/removes all values from data structure bringing it to initial state
    virtual void clear() = 0;

    // Returns value at first index
    virtual Type first_value() = 0;
    // Returns value at last index
    virtual Type last_value() = 0;
    // Returns value at given index
    virtual Type value_at(unsigned int position) = 0;

    // Returns number of values stored inside data structure
    virtual unsigned int get_size() = 0;
    // Returns size in bytes of whole data structure with data it stores
    virtual unsigned int get_byte_size() = 0;
    // Changes value at given index
    virtual void change_at(Type value, unsigned int position) = 0;
    // Returns data structure representation as string (useful for displaying)
    virtual std::string get_as_string() = 0;

};
#endif
