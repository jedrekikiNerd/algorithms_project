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

template <typename Type>
class DirectedWeightedGraph
{
public:
    std::vector<Vertex<Type>*> vertices;
    std::vector<Edge<Type>*> edges;

    DirectedWeightedGraph() {}

    ~DirectedWeightedGraph()
    {
        for (Vertex<Type>* vertex : vertices)
            delete vertex;
        for (Edge<Type>* edge : edges)
            delete edge;
    }

    void add_vertex(Type& data, std::string label)
    {
        size_t id = vertices.size();
        Vertex<Type>* new_vertex = new Vertex<Type>(data, label, id);
        vertices.push_back(new_vertex);
    }

    void add_edge(Vertex<Type>* source, Vertex<Type>* destination, int weight)
    {
        Edge<Type>* new_edge = new Edge<Type>(source, destination, weight);
        source->outgoing_edges.push_back(new_edge);
        destination->incoming_edges.push_back(new_edge);
        edges.push_back(new_edge);
    }

    Vertex<Type>* add_vertex(size_t id)
    {
        return vertices[id];
    }

    void get_as_string()
    {
        for (auto vertex : vertices)
        {
            std::cout << "Vertex " << vertex->data << " is connected to:\n";
            for (auto neighbor : vertex->neighbors)
            {
                std::cout << "\tVertex " << neighbor.first->data << " with weight " << neighbor.second << "\n";
            }
            std::cout << "\n";
        }
    }s
};
