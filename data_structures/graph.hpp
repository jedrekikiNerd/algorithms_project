#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <limits>
#include <algorithm>
#include "list_double.h"
#include "nodes.h"

class Vertex;

class Edge
{
public:
    Vertex *from;
    Vertex *to;
    double weight;
    std::vector<DoubleNode<Edge>*> references_to_self;
    Edge(Vertex* f, Vertex* t, double w) : from(f), to(t), weight(w) {}
};

class Vertex
{
public:
    int id;
    std::string label;
    std::string data;
    DoubleListHT<Edge*> neighbors;

    Vertex(int i, std::string l, std::string d = "") : id(i), label(l), data(d) {}
};

class DirectedWeightedGraph
{
private:
    std::vector<Vertex> vertices;
    DoubleListHT<Edge> edges;
    std::mt19937 gen;

public:
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
        for(int i=0; i<vertices[id].neighbors.get_size(); i++)
        {
            // remove every edge reference and edge itself
        }

        // Delete given vertex
        vertices.erase(vertices.begin() + id);

        // Update indexes of vertices
        for (Vertex &v : vertices)
        {
            // Every bigger id has to be decreased by one
            if (v.id > id)
            {
                (v.id)--;
            }
        }
    }

    void add_edge(int fromId, int toId, double weight)
    {
        Edge new_edge(&vertices[fromId], &vertices[toId], weight);
        new_edge.references_to_self.push_back(edges.add_back_special(new_edge));
        new_edge.references_to_self.push_back(vertices[fromId].neighbors.add_back_special(&edges.last_value()));
    }

    void remove_edge(int fromId, int toId)
    {
        
    }

    double generate_random_weight()
    {
        std::uniform_real_distribution<double> dis(1.0, 10.0);
        return dis(gen);
    }

    bool areAdjacent(Vertex& v, Vertex& w)
    {
        for (Edge* edge : v.neighbors)
        {
            if (edge->to->id == w.id)
                return true;
        }
        return false;
    }

    void replace_data(Vertex& v, std::string& x)
    {
        v.data = x;
    }

    void replace(Vertex& v, std::string& x)
    {
        v.label = x;
    }

    void replace_weight(Edge& e, double x)
    {
        e.weight = x;
    }

    std::string get_as_string()
    {
        std::string output = "";
        for (Vertex& vertex : vertices)
        {
            output += "Vertex " + vertex.label + " (" + vertex.id + ") - Data: " + vertex.data + "\n";
            for (Edge* neighbor : vertex.neighbors)
            {
                output += "  connects to Vertex " + neighbor->to->label + " (" + neighbor->to->id + ") with weight " + neighbor->weight + "\n";
            }
            output += "\n";
        }
        return output;
    }
};