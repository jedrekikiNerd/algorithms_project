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
    DoubleNode<Edge<Type>*>* node_in_source;
    DoubleNode<Edge<Type>*>* node_in_dest;
    DoubleNode<Edge<Type>>* node_in_graph;
    Edge(Vertex<Type>* f, Vertex<Type>* t, double w) : from(f), to(t), weight(w) {}
};

template <typename Type>
class Vertex
{
public:
    int id;
    std::string label;
    std::string data;
    DoubleListHT<Edge<Type>*> outgoing;  // Edges going out from this vertex
    DoubleListHT<Edge<Type>*> incoming;  // Edges coming into this vertex

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
        DoubleNode<Edge<Type>*>* node = vertex.outgoing.first_node();

        while(node != nullptr)
        {
            Edge<Type>* edge = node->value;
            remove_edge(id, edge->to->id);
            node=node->next_element;
        }

        node = vertex.incoming.first_node();

        while(node != nullptr)
        {
            Edge<Type>* edge = node->value;
            remove_edge(edge->from->id, id);
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
        DoubleNode<Edge<Type>>* ref = edges.add_back_special(new_edge);
        edges.last_value().node_in_graph = ref;
        edges.last_value().node_in_source = vertices[fromId].outgoing.add_back_special(&edges.last_value());
        edges.last_value().node_in_dest = vertices[toId].incoming.add_back_special(&edges.last_value());
    }

    void remove_edge(int from_id, int to_id)
    {
        Vertex<Type>& from_vertex = vertices[from_id];
        Vertex<Type>& to_vertex = vertices[to_id];

        // Find edge in the outgoing list and remove it
        DoubleNode<Edge<Type>*>* outgoing_node = from_vertex.outgoing.first_node();
        while (outgoing_node != nullptr)
        {
            if (outgoing_node->value->to->id == to_id)
            {
                std::cout << "Ba!";
                outgoing_node->value->to->incoming.remove_given(outgoing_node->value->node_in_dest);
                from_vertex.outgoing.remove_given(outgoing_node->value->node_in_source);
                edges.remove_given(outgoing_node->value->node_in_graph);
                break;
            }
            outgoing_node = outgoing_node->next_element;
        }
        std::cout << "Bangla!";
    }

    double generate_random_weight()
    {
        std::uniform_real_distribution<double> dis(1.0, 10.0);
        return dis(gen);
    }

    bool areAdjacent(Vertex<Type>& v, Vertex<Type>& w)
    {
        DoubleNode<Edge<Type>*>* node = v.outgoing.first_node();
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
            DoubleNode<Edge<Type>*>* node = vertex.outgoing.first_node();
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