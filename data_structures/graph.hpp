#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <limits>
#include <algorithm>

class Vertex;

class Edge
{
public:
    Vertex *from;
    Vertex *to;
    double weight;
    Edge(Vertex* f, Vertex* t, double w) : from(f), to(t), weight(w) {}
};

class Vertex
{
public:
    int id;
    std::string label;
    std::string data;
    std::vector<Edge> neighbors;

    Vertex(int i, std::string l, std::string d = "") : id(i), label(l), data(d) {}
};

class DirectedWeightedGraph
{
private:
    std::vector<Vertex> vertices;
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
        vertices.erase(vertices.begin() + id);
        for (Vertex& vertex : vertices)
        {
            vertex.id = &vertex - &vertices[0];
            vertex.neighbors.erase(std::remove_if(vertex.neighbors.begin(), vertex.neighbors.end(),
                [&vertices, id](Edge& edge) {
                    return edge.to->id == id || edge.to >= &vertices.back();
                }), vertex.neighbors.end());
        }
    }

    void add_edge(int fromId, int toId, double weight)
    {
        vertices[fromId].neighbors.push_back(Edge(&vertices[fromId], &vertices[toId], weight));
    }

    void remove_edge(int fromId, int toId)
    {
        std::vector<Edge>& neighbors = vertices[fromId].neighbors;
        neighbors.erase(std::remove_if(neighbors.begin(), neighbors.end(), [&vertices, toId](Edge& edge)
        {
            return edge.to->id == toId;
        }), neighbors.end());
    }

    std::array<Vertex*, 2> end_vertices(Edge& e)
    {
        return {e.from, e.to};
    }

    Vertex* opposite(Vertex& v, Edge& e)
    {
        if (e.from->id == v.id)
            return e.to;
        else if (e.to->id == v.id)
            return e.from;
        else
            return nullptr;
    }

    bool areAdjacent(Vertex& v, Vertex& w)
    {
        for (Edge& edge : v.neighbors)
        {
            if (edge.to->id == w.id)
                return true;
        }
        return false;
    }

    void replace_data(Vertex& v, const std::string& x)
    {
        v.data = x;
    }

    void replace(Vertex& v, const std::string& x)
    {
        v.label = x;
    }

    void replace_weight(Edge& e, double x)
    {
        e.weight = x;
    }

    double generate_random_weight()
    {
        std::uniform_real_distribution<double> dis(1.0, 10.0);
        return dis(gen);
    }

    void print_graph()
    {
        for (const Vertex& vertex : vertices)
        {
            std::cout << "Vertex " << vertex.label << " (" << vertex.id << ") - Data: " << vertex.data << std::endl;
            for (const Edge& neighbor : vertex.neighbors)
            {
                std::cout << "  connects to Vertex " << neighbor.to->label << " (" << neighbor.to->id << ") with weight " << neighbor.weight << std::endl;
            }
        }
    }
};
