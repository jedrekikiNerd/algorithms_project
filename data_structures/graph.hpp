#ifndef GRAPHS
#define GRAPHS

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <limits>
#include <algorithm>
#include "list_double.hpp"

template <typename Type>
class Vertex;

template <typename Type>
class Edge
{
public:
    Vertex<Type> *from;
    Vertex<Type> *to;
    double weight;
    DoubleNode<Edge<Type>>* reference_to_self;
    DoubleListHT<Edge<Type>>* reference_to_edges_list;
    DoubleNode<Edge<Type>*>* reference_to_self_fromv;
    DoubleListHT<Edge<Type>*>* reference_to_list_fromv;
    Edge(Vertex<Type>* f, Vertex<Type>* t, double w) : from(f), to(t), weight(w) {}
};

template <typename Type>
class Vertex
{
public:
    int id;
    std::string label;
    std::string data;
    DoubleListHT<Edge<Type>*> neighbors;

    Vertex(int i, std::string l, Type d) : id(i), label(l), data(d) {}
};

template <typename Type>
class DirectedWeightedGraph
{
private:
    std::vector<Vertex<Type>> vertices;
    DoubleListHT<Edge<Type>> edges;
    std::mt19937 gen;

public:
    DirectedWeightedGraph() {}
    DirectedWeightedGraph(int numVertices, double density) : gen(std::random_device{}())
    {
        for (int i = 0; i < numVertices; i++)
        {
            add_vertex("Vertex " + std::to_string(i));
        }

        std::uniform_real_distribution<double> dis(0.0, 1.0);
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                if (i != j && dis(gen) < density)
                {
                    add_edge(i, j, generate_random_weight());
                }
            }
        }
    }

    void add_vertex(const std::string& label, const std::string& data = "")
    {
        vertices.emplace_back(vertices.size(), label, data);
    }

    void remove_vertex(int id)
    {
        Vertex<Type>& vertex = vertices[id];
        DoubleNode<Edge<Type>*>* node = vertex.neighbors.first_node();

        // Usuwanie krawędzi odniesienia do tego wierzchołka
        while(node != nullptr)
        {
            Edge<Type>* edge = node->value;
            size_t i = 0;
            edge->references_to_self->clear();
            edge->reference_to_self_list->remove_given(edge->reference_to_self);
            node=node->next_element;
        }

        // Delete given vertex
        vertices.erase(vertices.begin() + id);

        // Update indexes of vertices
        for (Vertex<Type> &v : vertices)
        {
            // Every bigger id has to be decreased by one
            if (v.id > id)
            {
                v.id--;
            }
        }
    }

    void add_edge(int fromId, int toId, double weight)
    {
        Edge<Type> new_edge(&vertices[fromId], &vertices[toId], weight);
        new_edge.reference_to_self_list = &edges;
        DoubleNode<Edge<Type>>* ref = edges.add_back_special(new_edge);
        edges.last_value().reference_to_self = ref;
        edges.last_value().reference_to_edges_list = &edges;
    }

    void remove_edge(int fromId, int toId)
    {
        Vertex<Type>& from_vertex = vertices[fromId];
        DoubleNode<Edge<Type>*>* node = from_vertex.neighbors.first_node();

        while(node != nullptr)
        {
            if (node->value->to->id == toId)
            {
                Edge<Type>* edge = node->value;
                size_t i = 0;
                for (DoubleNode<Edge<Type>*>* ref : edge->references_to_self)
                {
                    if (ref != nullptr)
                    {
                        DoubleNode<Edge<Type>*>* prev_node = ref->previous_element;
                        DoubleNode<Edge<Type>*>* next_node = ref->next_element;

                        if (prev_node != nullptr)
                            prev_node->next_element = next_node;
                        if (next_node != nullptr)
                            next_node->previous_element = prev_node;
                        
                        delete ref;
                    }
                    edge->references_to_self_lists[i]->size--;
                    i++;
                }
                edge->reference_to_self_list->remove_given(edge->reference_to_self);
                break;
            }
            node=node->next_element;
        }
    }

    double generate_random_weight()
    {
        std::uniform_real_distribution<double> dis(1.0, 10.0);
        return dis(gen);
    }

    bool areAdjacent(Vertex<Type>& v, Vertex<Type>& w)
    {
        DoubleNode<Edge<Type>*>* node = v.neighbors.first_node();
        while(node != nullptr)
        {
            if (node->value->to->id == w.id)
                return true;
        }
        return false;
    }

    void replace_data(Vertex<Type>& v, Type& x)
    {
        v.data = x;
    }

    void replace(Vertex<Type>& v, std::string& x)
    {
        v.label = x;
    }

    void replace_weight(Edge<Type>& e, double x)
    {
        e.weight = x;
    }

    std::string get_as_string()
    {
        std::string output = "";
        for (Vertex<Type>& vertex : vertices)
        {
            output += "Vertex " + vertex.label + " (" + std::to_string(vertex.id) + ") - Data: " + vertex.data + "\n";
            DoubleNode<Edge<Type>*>* node = vertex.neighbors.first_node();
            while(node != nullptr)
            {
                Edge<Type>* neighbor = node->value;
                output += "  connects to Vertex " + neighbor->to->label + " (" + std::to_string(neighbor->to->id) + ") with weight " + std::to_string(neighbor->weight) + "\n";
                node = node->next_element;
            }
            output += "\n";
        }
        return output;
    }

    // Return size of graphin bytes
    unsigned int get_byte_size()
    {
        return sizeof(DirectedWeightedGraph) + sizeof(vertices) + sizeof(edges);
    }
};

#endif