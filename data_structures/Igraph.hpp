#ifndef GRAPHI
#define GRAPHI
#include "graph_parts.hpp"
#include <iostream>

template <typename Type>
class GraphInterface
{
public:

    virtual void generate_graph(int num_vertices, float density, Type default_data) = 0;

    ~GraphInterface() {}

    virtual void add_vertex(Type& data, std::string label) = 0;

    virtual void add_edge(Vertex<Type>* source, Vertex<Type>* destination, int weight) = 0;

    virtual Vertex<Type>* get_vertex(size_t id) = 0;

    virtual void remove_vertex(Vertex<Type>* vertex) = 0;

    virtual void remove_edge(Vertex<Type>* source, Vertex<Type>* destination) = 0;

    virtual std::string get_as_string() = 0;

    virtual bool are_adjacent(Vertex<Type>* v, Vertex<Type>* w) = 0;

    virtual void replace_data(Vertex<Type>* v, Type& x) = 0;

    virtual void replace(Vertex<Type>* v, std::string x) = 0;

    virtual void replace_weight(Edge<Type>* e, int x) = 0;

    virtual unsigned int get_byte_size() = 0;

    virtual size_t get_size() = 0;

    virtual std::string shortest_paths(size_t src) = 0;

    virtual std::string shortest_path_to(size_t src, size_t dest) = 0;
};

#endif