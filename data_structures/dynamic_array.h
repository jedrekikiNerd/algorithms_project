#ifndef DARRAY
#define DARRAY
#include "I_data_structure.h"
#include <iostream>
#include <type_traits>
#include "film_struct.h"

template <typename Type>
class DynamicArray : public IDataStructure<Type>
{
private:
    Type *arr;
    unsigned int capacity;
    unsigned int size;

public:
    DynamicArray() 
    {
        capacity = 1;
        size = 0;
        arr = new Type[capacity];
    }

    DynamicArray(int cap) 
    {
        capacity = cap;
        size = 0;
        arr = new Type[capacity];
    }

    ~DynamicArray() 
    {
        delete[] arr;
    }

    void add_back(Type value) 
    {
        if (size >= capacity)
        {
            // If the array is full, double its size
            unsigned int newCapacity = capacity * 2;
            Type *tempArr = new Type[newCapacity];
            for (unsigned int i = 0; i < size; ++i)
            {
                tempArr[i] = arr[i];
            }
            delete[] arr;
            arr = tempArr;
            capacity = newCapacity;
        }
        arr[size] = value;
        size++;
    }

    void add_front(Type value)
    {
        if (size >= capacity)
        {
            // If the array is full, double its size
            unsigned int newCapacity = capacity * 2;
            Type *tempArr = new Type[newCapacity];
            for (unsigned int i = 0; i < size; ++i)
            {
                tempArr[i+1] = arr[i];
            }
            delete[] arr;
            arr = tempArr;
            capacity = newCapacity;
        }
        else
        { 
            // Shift all elements by one
            for(unsigned int i = size; i > 0; i--)
            {
                arr[i] = arr[i-1];
            }
        }
        arr[0] = value;
        size++;
    }

    void add_at(Type value, unsigned int position)
    {
        if (position >= 0 && position <= size)
        {
            if (size >= capacity) 
            {
                // If the array is full, double its size
                unsigned int newCapacity = capacity * 2;
                Type *tempArr = new Type[newCapacity];
                for (unsigned int i = 0; i < position ; ++i)
                {
                    tempArr[i] = arr[i];
                }
                tempArr[position] = value;
                for (unsigned int i = position; i < size; i++)
                {
                    tempArr[i+1] = arr[i];
                }
                delete[] arr;
                arr = tempArr;
                capacity = newCapacity;
            }
            else
            {
                for (unsigned int i = size; i >position; i--)
                {
                    arr[i] = arr[i-1];
                }
                arr[position] = value;
            }
        size++;
        }
    }

    void remove_front()
    {
        if (size > 0)
        {
        if(size-1 <= capacity/2 && capacity > 1 ){
            unsigned int newCapacity = capacity /2;
            Type *tempArr = new Type[newCapacity];
            for (unsigned int i=0; i < size-1;i++){
                tempArr[i] = arr[i+1];
            }
            delete[] arr;
            arr = tempArr;
            capacity = newCapacity;
        } else {
            for(unsigned int i=0; i < size-1; i++){
                arr[i] = arr[i+1];
            }  
        }
        size--;
        }
    }

    void remove_back() 
    { 
        if (size > 0)
        {
            if (size - 1 <= capacity / 2 && capacity > 1)
            {
                unsigned int newCapacity = capacity / 2;
                Type *tempArr = new Type[newCapacity];
                for (unsigned int i =0; i < size - 1; i++)
                {
                    tempArr[i] = arr[i];
                }
                delete[] arr;
                arr = tempArr;
                capacity = newCapacity;
            }
        size--;
        }
    }

    void remove_at(unsigned int position) 
    {
        if (size > 0 && position >= 0 && position <= size)
        {
            if (size - 1 <= capacity / 2 && capacity > 1)
            {
                unsigned int newCapacity = capacity / 2;
                Type *tempArr = new Type[newCapacity];

                for (unsigned int i = 0; i < position; i++)
                {
                    tempArr[i] = arr[i];
                }

                for (unsigned int i = position; i < size - 1; i++)
                {
                    tempArr[i] = arr[i+1];
                }
                delete[] arr;
                arr = tempArr;
                capacity = newCapacity;
            }
            else
            {
                for (unsigned int i = position; i < size - 1;i++)
                {
                    arr[i] = arr[i+1];
                }
            }
            size--;
        }
    }

    void clear()
    {
        delete[] arr;
        capacity = 1;
        size = 0;
        arr = new Type[capacity];
    }

    Type& operator[](unsigned int position)
    {
        return arr[position];
    };

    unsigned int get_size()
    {
        return size;
    }

    Type first_value()
    {
        return arr[0];
    }

    Type last_value()
    {
        return arr[size-1];
    }

    Type value_at(unsigned int position)
    {
        if (position < 0 || position >= size)
            throw std::out_of_range("Position out of range");
        return arr[position];
    }

    unsigned int get_byte_size()
    {
        return sizeof(DynamicArray) + sizeof(Type)*capacity;
    }

    void change_at(Type value, unsigned int position)
    {
        if (position < 0 || position >= size)
            return;
            //throw std::out_of_range("Position out of range");
        arr[position] = value;
    }

    // Returns string representation of data inside list
    std::string get_as_string()
    {
        return "";
    }
};

// Speciality for film_struct
template<>
inline std::string DynamicArray<film_struct>::get_as_string() {
    std::string output = "DynamicArray[";
    for (unsigned int i = 0; i < size; i++)
    {
        output += "<";
        output += arr[i].title + "," + std::to_string(arr[i].rank) + ">";
        if ( i < size-1 )
            output += ", ";
    }
    output += "]";
    return output;
}


#endif