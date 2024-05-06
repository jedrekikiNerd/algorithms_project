#ifndef GRAPHPARTS
#define GRAPHPARTS
#include <iostream>
#include <vector>

template <typename Type>
class Vertex;

template <typename Type>
class Edge
{
public:
    Vertex<Type>* source;
    Vertex<Type>* destination;
    int weight;
    size_t id;

    Edge(Vertex<Type>* source, Vertex<Type>* destination, int weight) : source(source), destination(destination), weight(weight) {}
};

template <typename Type>
class Vertex
{
public:
    Type data;
    std::string label;
    size_t id;
    std::vector<Edge<Type>*> outgoing_edges;
    std::vector<Edge<Type>*> incoming_edges;

    Vertex(Type& data, std::string label, size_t id) : data(data), label(label), id(id) {}
};

#endif