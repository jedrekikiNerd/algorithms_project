#ifndef LISTHT
#define LISTHT

#include <iostream>
#include <stdexcept>
#include "I_data_structure.h"
#include <type_traits>
#include "nodes.h"


/**
 * SingleListHT class that represents a list data structure with methods
 * 
 */
template <typename Type>
class SingleListHT : public IDataStructure<Type>
{
private:
    Node<Type>* head;
    Node<Type>* tail;
    unsigned int size;


public:
    SingleListHT() : head(nullptr), tail(nullptr), size(0) {}
    ~SingleListHT() {
        clear();
    }

    // Adds element where head ptr is pointing
    void add_front(Type value)
    {
        Node<Type>* new_node = new Node<Type>(value);
        if (head == nullptr)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            new_node->next_element = head;
            head = new_node;
        }
        size++;
    }

    // Adds element to the tail of the list
    void add_back(Type value)
    {
        Node<Type>* new_node = new Node<Type>(value);
        if (head == nullptr)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next_element = new_node;
            tail = new_node;
            
        }
        size++;
    }

    // Adds element on specified index starting counting from head
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

            Node<Type>* new_node = new Node<Type>(value);

            Node<Type>* currrent_node = head;
            for(unsigned int i=1; i<position; i++)
            {
                currrent_node = currrent_node->next_element;
            }
            new_node->next_element = currrent_node->next_element;
            currrent_node->next_element = new_node;
            size++;
        }
    }

    // Removes first element - based on head pointer
    void remove_front()
    {
        if (head != nullptr) 
        {
            Node<Type>* temp = head;
            head = head->next_element;
            delete temp;
            if(head == nullptr)
                tail = nullptr;
            size--;
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
                Node<Type>* prev_last_node = head;
                // We have to reach last element - 1 in order to change pointers and then delete last one
                while(prev_last_node->next_element != tail)
                {
                    prev_last_node = prev_last_node->next_element;
                }
                // We delete last element and set nullptr for prev_last
                delete tail;
                tail = prev_last_node;
                tail->next_element = nullptr;
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

            Node<Type>* currrent_node = head;
            for(unsigned int i=1; i<position; i++)
            {
                currrent_node = currrent_node->next_element;
            }
            Node<Type>* temp = currrent_node->next_element;
            currrent_node->next_element = temp->next_element;
            delete temp;
            if (currrent_node->next_element == nullptr)
                tail = currrent_node;
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
        if (position >= size or position < 0)
            throw std::out_of_range("Index is out of range");
        else
        {
            if(position == size-1)
                return tail->value;
            Node<Type>* current_node = head;
            for(unsigned int i=0; i<position; i++)
                current_node=current_node->next_element;
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
        Node<Type>* current_node = head;
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
        Node<Type>* current_node = head;
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
        return sizeof(SingleListHT) + sizeof(Node<Type>)*size;
    }

    // Change value at given position
    void change_at(Type value, unsigned int position)
    {
        if (position < 0 || position >= size)
            return;
            //throw std::out_of_range("Position out of range");
    
        if(position == size-1)
        {
            tail->value = value;
            return;
        }

        Node<Type>* current_node = head;

        for(unsigned int i=0; i<position; i++)
            current_node=current_node->next_element;
        
        current_node->value = value;
    }
};
#endif
