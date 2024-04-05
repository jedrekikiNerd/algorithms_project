#ifndef NODES
#define NODES


/**
 * Node class for single container used by basic list
 * It contains pointer to next container and stores value of type read from template
 * 
 */
template <typename Type>
class Node
{
public:
    Type value;
    Node<Type>* next_element;

    Node(Type value) : value(value), next_element(nullptr) {}
};


/**
 * DoubleNode class for single container used by double linked list
 * It contains pointer to next and previous container and stores value of type read from template
 * 
 */
template <typename Type>
class DoubleNode
{
public:
    Type value;
    DoubleNode<Type>* next_element;
    DoubleNode<Type>* previous_element;

    DoubleNode(Type value) : value(value), next_element(nullptr), previous_element(nullptr) {}
};

#endif