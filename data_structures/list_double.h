#ifndef DLISTHT
#define DLISTHT

#include <iostream>
#include <stdexcept>
#include "I_data_structure.h"
#include <type_traits>
#include "nodes.h"


/**
 * LinkedListHT class that represents a list data structure with methods
 * 
 */
template <typename Type>
class DoubleListHT : public IDataStructure<Type>
{
private:
    DoubleNode<Type>* head;
    DoubleNode<Type>* tail;
    unsigned int size;


public:
    DoubleListHT() : head(nullptr), tail(nullptr), size(0) {}
    ~DoubleListHT() {
        clear();
    }

    // Adds element where head ptr is pointing
    void add_front(Type value)
    {
        DoubleNode<Type>* new_node = new DoubleNode<Type>(value);
        if (head == nullptr)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            new_node->next_element = head;
            head->previous_element = new_node;
            head = new_node;
        }
        size++;
    }

    // Adds element to the tail of the list
    void add_back(Type value)
    {
        DoubleNode<Type>* new_node = new DoubleNode<Type>(value);
        if (head == nullptr)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next_element = new_node;
            new_node->previous_element = tail;
            tail = new_node;  
        }
        size++;
    }

    // Adds element on specified index starting counting from head or from tail
    void add_at(Type value, unsigned int position)
    {
        if (position > size or position < 0)
        {
            std::cerr << "Position exceeds size!";
            return;
        }
        else
        {
            if (position == 0)
            {
                add_front(value);
                return;
            }

            if (position == size)
            {
                add_back(value);
                return;
            }

            DoubleNode<Type>* new_node = new DoubleNode<Type>(value);

            if(position <= size/2)
            {
                DoubleNode<Type>* currrent_node = head;
                for(unsigned int i=1; i<position; i++)
                {
                    currrent_node = currrent_node->next_element;
                }
                new_node->next_element = currrent_node->next_element;
                new_node->next_element->previous_element = new_node;
                new_node->previous_element = currrent_node;
                currrent_node->next_element = new_node;
            }
            else
            {
                DoubleNode<Type>* currrent_node = tail;
                for(unsigned int i=size-1; i>position; i--)
                {
                    currrent_node = currrent_node->previous_element;
                }
                new_node->next_element = currrent_node;
                new_node->previous_element = currrent_node->previous_element;
                currrent_node->previous_element = new_node;
                new_node->previous_element->next_element = new_node;
            }
            size++;
        }
    }

    // Removes first element - based on head pointer
    void remove_front()
    {
        if (head != nullptr) 
        {
            DoubleNode<Type>* temp = head;
            head = head->next_element;
            delete temp;
            size--;
            if(head == nullptr)
            {
                tail = nullptr;
                return;
            }
            head->previous_element = nullptr;
        }
    }

    // Removes last element - needs to iterate from head to tail
    void remove_back()
    {
        if (head != nullptr)
        {
            if (head == tail)
            {
                delete head;
                tail = nullptr;
                head = nullptr;
            }
            else
            {
                // We delete last element and set nullptr to previous one
                DoubleNode<Type>* temp = tail;
                tail = tail->previous_element;
                tail->next_element = nullptr;
                delete temp;
            }
            size--;
        }
    }

    // Removes element at specified position counting from head (0)
    void remove_at(unsigned int position)
    {
        if (position >= size or position < 0)
        {
            std::cerr << "Position exceeds size!";
            return;
        }
        else
        {
            if (position == 0)
            {
                remove_front();
                return;
            }
            if (position == size-1)
            {
                remove_back();
                return;
            }

            DoubleNode<Type>* current_node = nullptr;
            if(position <= size/2)
            {
                current_node = head;
                for(unsigned int i = 0; i < position; i++)
                {
                current_node = current_node->next_element;
                }
            }
            else
            {
                current_node = tail;
                for(unsigned int i = size-1; i > position; i--)
                {
                current_node = current_node->previous_element;
                }
            }
            DoubleNode<Type>* prev_node = current_node->previous_element;
            DoubleNode<Type>* next_node = current_node->next_element;

            if (prev_node != nullptr)
                prev_node->next_element = next_node;
            if (next_node != nullptr)
                next_node->previous_element = prev_node;

            delete current_node;
            size--;
        }
    }

    // Clears all nodes stored by this list, useful for deconstructing to free all memory
    void clear()
    {
        while(head)
            this->remove_front();
    }

    // Returns first value (head value)
    Type first_value()
    {
        if (head==nullptr)
            throw std::out_of_range("Index is out of range");
        return head->value;
    }

    // Returns last value (tail value)
    Type last_value()
    {
        if (tail==nullptr)
            throw std::out_of_range("Index is out of range");
        return tail->value;
    }

    Type value_at(unsigned int position)
    {
        if (position >= size)
            throw std::out_of_range("Index is out of range");
        else
        {
            DoubleNode<Type>* current_node = nullptr;
            if(position <= size/2)
            {
                current_node = head;
                for(unsigned int i=0; i<position; i++)
                    current_node=current_node->next_element;
            }
            else
            {
                current_node = tail;
                for(unsigned int i=size-1; i>position; i--)
                    current_node=current_node->previous_element;
            }
                
            return current_node->value;
        }
    }

    // Returns size of list
    unsigned int get_size()
    {
        return size;
    }

    // Returns position of element
    unsigned int find(Type value)
    {
        DoubleNode<Type>* current_node = head;
        unsigned int i = 0;
        while(current_node != nullptr)
        {
            if (current_node->value == value)
                return i;
            i++;
            current_node = current_node->next_element;
        }
        return -1;
    }

    // Returns string representation of data inside list
    std::string get_as_string()
    {
        std::string output = "List[";
        DoubleNode<Type>* current_node = head;
        if (std::is_integral_v<Type> != true)
            return "ERROR: typename of this list is not supported by this method!";
        while(current_node != nullptr)
        {
            output += std::to_string(current_node->value);
            current_node = current_node->next_element;
            if (current_node != nullptr)
                output += ", ";
        }
        output += "]";
        return output;
    }

    // Return size of data structure in bytes
    unsigned int get_byte_size()
    {
        return sizeof(DoubleListHT) + sizeof(DoubleNode<Type>)*size;
    }

    // Change value at given position
    void change_at(Type value, unsigned int position)
    {
        if (position < 0 || position >= size)
            return;
            //throw std::out_of_range("Position out of range");
        
        DoubleNode<Type>* current_node = nullptr;
        if(position <= size/2)
        {
            current_node = head;
            for(unsigned int i=0; i<position; i++)
                current_node=current_node->next_element;
        }
        else
        {
            current_node = tail;
            for(unsigned int i=size-1; i>position; i--)
                current_node=current_node->previous_element;
        }
        
        current_node->value = value;
    }
};
#endif
