#ifndef GRAPHS
#define GRAPHS

#include <iostream>
#include <vector>
#include <random>
#include <queue>

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

template <typename Type>
class DirectedWeightedGraph
{
public:
    std::vector<Vertex<Type>*> vertices;
    std::vector<Edge<Type>*> edges;

    DirectedWeightedGraph() {}

    void generate_graph(int num_vertices, float density, Type default_data)
    {
        // Add given number of vertices
        for (int i = 0; i < num_vertices; ++i)
        {
            std::string label = "Vertex_" + std::to_string(i);
            this->add_vertex(default_data, label);
        }

        int num_of_edges = 0;

        // Random generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        // Adding edges with probability of given density
        for (size_t i = 0; i < num_vertices; i++)
        {
            for (size_t j = i + 1; j < num_vertices; j++)
            {
                if (dis(gen) < density)
                {
                    // Calculate random weight
                    int weight = std::uniform_int_distribution<>(1, 100)(gen);

                    // Adding edge
                    this->add_edge(this->get_vertex(i), this->get_vertex(j), weight);
                    this->add_edge(this->get_vertex(j), this->get_vertex(i), weight);
                }
            }
        }
    }

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
        new_edge->id = edges.size();
        source->outgoing_edges.push_back(new_edge);
        destination->incoming_edges.push_back(new_edge);
        edges.push_back(new_edge);
    }

    Vertex<Type>* get_vertex(size_t id)
    {
        return vertices[id];
    }

    void remove_vertex(Vertex<Type>* vertex)
    {
        // Remove incoming edges
        for (size_t i = 0; i < vertex->incoming_edges.size(); ++i)
        {
            Edge<Type>* edge = vertex->incoming_edges[i];
            Vertex<Type>* source = edge->source;

            // Remove edge from outgoing edges in source vertex
            for (size_t j = 0; j < source->outgoing_edges.size(); ++j)
            {
                if (source->outgoing_edges[j] == edge)
                {
                    source->outgoing_edges.erase(source->outgoing_edges.begin() + j);
                    break;
                }
            }
            // Reindex edges ids
            for (size_t i = (edge->id)+1; i < edges.size(); i++)
            {
                edges[i]->id = i-1;
            }
            edges.erase(edges.begin() + edge->id);
            delete edge;
        }
        vertex->incoming_edges.clear();

        // Delete edges that go out from our vertex
        for (size_t i = 0; i < vertex->outgoing_edges.size(); ++i)
        {
            Edge<Type>* edge = vertex->outgoing_edges[i];
            Vertex<Type>* destination = edge->destination;

            // Delete edges in destination vertex incoming edges
            for (size_t j = 0; j < destination->incoming_edges.size(); ++j)
            {
                if (destination->incoming_edges[j] == edge)
                {
                    destination->incoming_edges.erase(destination->incoming_edges.begin() + j);
                    break;
                }
            }
            // Reindex edges ids
            for (size_t i = (edge->id)+1; i < edges.size(); i++)
            {
                edges[i]->id = i-1;
            }
            edges.erase(edges.begin() + edge->id);
            delete edge;
        }
        vertex->outgoing_edges.clear();

        // Reindex vertices ids
        for (size_t i = (vertex->id)+1; i < vertices.size(); i++)
        {
            vertices[i]->id = i-1;
        }

        vertices.erase(vertices.begin() + vertex->id);

        // Delete vertex pointer
        delete vertex;
    }

    void remove_edge(Vertex<Type>* source, Vertex<Type>* destination)
    {
        // Find pointer to edge between these two vertexes
        Edge<Type>* edge_to_remove = nullptr;
        for (size_t i = 0; i < source->outgoing_edges.size(); i++)
        {
            if (source->outgoing_edges[i]->destination == destination)
            {
                edge_to_remove = source->outgoing_edges[i];
                break;
            }
        }

        // If edge exists delete it
        if (edge_to_remove)
        {
            // Delete edge pointer from source vertex outgoing edges vector
            for (size_t i = 0; i < source->outgoing_edges.size(); i++)
            {
                if (source->outgoing_edges[i] == edge_to_remove)
                {
                    source->outgoing_edges.erase(source->outgoing_edges.begin() + i);
                    break;
                }
            }

            // Delete edge pointer from destination vertex incoming edges vector
            for (size_t i = 0; i < destination->incoming_edges.size(); i++)
            {
                if (destination->incoming_edges[i] == edge_to_remove)
                {
                    destination->incoming_edges.erase(destination->incoming_edges.begin() + i);
                    break;
                }
            }

            // Reindex edges ids
            for (size_t i = (edge_to_remove->id)+1; i < edges.size(); i++)
            {
                edges[i]->id = i-1;
            }
            edges.erase(edges.begin() + edge_to_remove->id);
            // Delete edge
            delete edge_to_remove;
        }
    }

    std::string get_as_string()
    {
        std::string output = "";
        for (Vertex<Type>* vertex : vertices)
        {
            output += "Vertex " + vertex->label + " (" + std::to_string(vertex->id) + ") - Data: " + vertex->data + "\n";
            for (Edge<Type>* out_edge : vertex->outgoing_edges)
            {
                output += "  connects to Vertex " + out_edge->destination->label + " (" + std::to_string(out_edge->destination->id) + ") with weight " + std::to_string(out_edge->weight) + "\n";
            }
            output += "\n";
        }
        return output;
    }

    // Return size of graph in bytes
    unsigned int get_byte_size()
    {
        return sizeof(DirectedWeightedGraph) + sizeof(vertices) + sizeof(edges);
    }

    std::string shortest_paths(size_t src)
    {
        std::priority_queue<std::pair<int, Vertex<Type>*>, std::vector<std::pair<int, Vertex<Type>*>>, std::greater<std::pair<int, Vertex<Type>*>>> pq;

        // Set distances as infinity
        std::vector<int> dist(vertices.size(), std::numeric_limits<int>::max());
        // Vector to store the paths
        std::vector<std::vector<size_t>> paths(vertices.size());

        // Distance to self set to 0
        pq.push({0, vertices[src]});
        dist[src] = 0;

        // Main loop of dijkstra algorithm
        while (!pq.empty())
        {
            int u = pq.top().second->id;
            pq.pop();

            // Go through edges that go out of vertex
            for (Edge<Type>* edge : vertices[u]->outgoing_edges)
            {
                int v = edge->destination->id;
                int weight = edge->weight;

                // If path through u is shorter than v
                if (dist[v] > dist[u] + weight)
                {
                    // Update distance
                    dist[v] = dist[u] + weight;
                    // Store the path
                    paths[v] = paths[u];
                    paths[v].push_back(v);
                    pq.push({dist[v], edge->destination});
                }
            }
        }

        std::string result = "";

        // Show distances and paths
        result += "Vertex Distance from Source\n";
        for (size_t i = 0; i < vertices.size(); i++)
        {
            result += "To id: (" + std::to_string(i) + ")\t\tDistance: " + std::to_string(dist[i]) + "\t\tPath: ";
            for (size_t j = 0; j < paths[i].size(); j++)
            {
                result += std::to_string(paths[i][j]) + " ";
            }
            result += "\n";
        }
        return result;
    }

std::string shortest_path_to(size_t src, size_t dest)
    {
        std::priority_queue<std::pair<int, Vertex<Type>*>, std::vector<std::pair<int, Vertex<Type>*>>, std::greater<std::pair<int, Vertex<Type>*>>> pq;

        // Set distances as infinity
        std::vector<int> dist(vertices.size(), std::numeric_limits<int>::max());
        // Vector to store the paths
        std::vector<std::vector<size_t>> paths(vertices.size());

        // Distance to self set to 0
        pq.push({0, vertices[src]});
        dist[src] = 0;

        // Main loop of dijkstra algorithm
        while (!pq.empty())
        {
            int u = pq.top().second->id;
            pq.pop();

            // Stop when destination is reached
            if (u == dest)
                break;

            // Go through edges that go out of vertex
            for (Edge<Type>* edge : vertices[u]->outgoing_edges)
            {
                int v = edge->destination->id;
                int weight = edge->weight;

                // If path through u is shorter than v
                if (dist[v] > dist[u] + weight)
                {
                    // Update distance
                    dist[v] = dist[u] + weight;
                    // Store the path
                    paths[v] = paths[u];
                    paths[v].push_back(v);
                    pq.push({dist[v], edge->destination});
                }
            }
        }

        std::cout << "Super jest!";

        std::string result = "";

        // Show distances and paths
        result += "Vertex Distance from Source\n";
        result += "To id: (" + std::to_string(dest) + ")\t\tDistance: " + std::to_string(dist[dest]) + "\t\tPath: ";
        for (size_t j = 0; j < paths[dest].size(); j++)
        {
            std::cout << j << std::endl;
            result += std::to_string(paths[dest][j]) + " ";
        }
        result += "\n";
        return result;
    }
};

#endif