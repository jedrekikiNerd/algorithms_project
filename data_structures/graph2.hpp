#ifndef GRAPH2
#define GRAPH2

#include <iostream>
#include <vector>
#include <random>
#include "graph_parts.hpp"
#include "Igraph.hpp"

template <typename Type>
class DirectedWeightedGraphIncidence : public GraphInterface<Type>
{
public:
    std::vector<std::vector<Edge<Type>*>> incidence_matrix;
    std::vector<Vertex<Type>*> vertices;

    DirectedWeightedGraphIncidence() {}

    void generate_graph(int num_vertices, float density, Type default_data)
    {
        // Reset incidence matrix
        incidence_matrix.clear();
        vertices.clear();

        // Add given number of vertices
        for (int i = 0; i < num_vertices; ++i)
        {
            vertices.push_back(new Vertex<Type>(default_data, "Vertex_" + std::to_string(i), i));
        }

        // Resize incidence matrix and initialize with nullptrs
        incidence_matrix.resize(num_vertices, std::vector<Edge<Type>*>(num_vertices, nullptr));

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
                    add_edge(vertices[i], vertices[j], weight);
                    add_edge(vertices[j], vertices[i], weight);
                }
            }
        }
    }

    Vertex<Type>* get_vertex(size_t id)
    {
        return vertices[id];
    }

    // Add edge between vertices
    void add_edge(Vertex<Type>* source, Vertex<Type>* destination, int weight)
    {
        size_t source_id = source->id;
        size_t destination_id = destination->id;
        Edge<Type>* new_edge = new Edge<Type>(vertices[source_id], vertices[destination_id], weight);
        incidence_matrix[source_id][destination_id] = new_edge;
        incidence_matrix[destination_id][source_id] = new_edge;
    }

    // Delete edge between vertices
    void remove_edge(Vertex<Type>* source, Vertex<Type>* destination)
    {
        size_t source_id = source->id;
        size_t destination_id = destination->id;

        if (incidence_matrix[source_id][destination_id] != nullptr)
        {
            delete incidence_matrix[source_id][destination_id];
            incidence_matrix[source_id][destination_id] = nullptr;
        }
    
        if (incidence_matrix[destination_id][source_id] != nullptr)
        {
            incidence_matrix[destination_id][source_id] = nullptr;
        }
    }

    // Add a new vertex to the graph
    void add_vertex(Type& data, std::string label)
    {
        size_t num_vertices = vertices.size();

        // Add a new vertex
        vertices.push_back(new Vertex<Type>(data, label, num_vertices));

        // Resize each row in incidence matrix and add nullptr for the new vertex
        for (std::vector<Edge<Type>*> &row : incidence_matrix)
            row.push_back(nullptr);

        // Add a new row with nullptrs for the new vertex
        incidence_matrix.push_back(std::vector<Edge<Type>*>(num_vertices + 1, nullptr));
    }

    // Remove a vertex from the graph
    void remove_vertex(Vertex<Type>* vertex)
    {
        size_t vertex_id = vertex->id;
        // Delete all edges connected to the vertex
        for (size_t i = 0; i < incidence_matrix.size(); i++)
        {
            if (incidence_matrix[vertex_id][i] != nullptr)
            {
                delete incidence_matrix[vertex_id][i];
                incidence_matrix[vertex_id][i] = nullptr;
            }

            if (incidence_matrix[i][vertex_id] != nullptr)
            {
                incidence_matrix[i][vertex_id] = nullptr;
            }
        }

        for(size_t i = vertex_id+1; i<vertices.size(); i++)
        {
            vertices[i]->id--;
        }

        // Delete the vertex object
        delete vertices[vertex_id];
        vertices.erase(vertices.begin() + vertex_id);

        // Erase the corresponding row and column from incidence matrix
        incidence_matrix.erase(incidence_matrix.begin() + vertex_id);
        for (std::vector<Edge<Type>*>& row : incidence_matrix)
            row.erase(row.begin() + vertex_id);
    }

    bool are_adjacent(Vertex<Type>* v, Vertex<Type>* w)
    {
        return false;
    }

    void replace_data(Vertex<Type>* v, Type& x)
    {
        v->data = x;
    }

    void replace(Vertex<Type>* v, std::string x)
    {
        v->label = x;
    }

    void replace_weight(Edge<Type>* e, int x)
    {
        e->weight = x;
    }

    // Return size of graph in bytes
    unsigned int get_byte_size()
    {
        return sizeof(DirectedWeightedGraphIncidence) + sizeof(vertices) + sizeof(incidence_matrix);
    }

    std::string shortest_paths(size_t src)
    {
        std::priority_queue<std::pair<int, size_t>, std::vector<std::pair<int, size_t>>, std::greater<std::pair<int, size_t>>> pq;

        // Set distances as infinity
        std::vector<int> dist(vertices.size(), std::numeric_limits<int>::max());
        // Vector to store the paths
        std::vector<std::vector<size_t>> paths(vertices.size());

        // Distance to self set to 0
        pq.push({0, src});
        dist[src] = 0;

        // Main loop of dijkstra algorithm
        while (!pq.empty())
        {
            size_t u = pq.top().second;
            pq.pop();

            // Go through edges incident to vertex u
            for (size_t v = 0; v < incidence_matrix[u].size(); ++v)
            {
                if (incidence_matrix[u][v] != 0)
                {
                    int weight = incidence_matrix[u][v]->weight;
                    
                    // If path through u is shorter than v
                    if (dist[v] > dist[u] + weight)
                    {
                        // Update distance
                        dist[v] = dist[u] + weight;
                        // Store the path
                        paths[v] = paths[u];
                        paths[v].push_back(v);
                        pq.push({dist[v], v});
                    }
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
        std::priority_queue<std::pair<int, size_t>, std::vector<std::pair<int, size_t>>, std::greater<std::pair<int, size_t>>> pq;

        // Set distances as infinity
        std::vector<int> dist(vertices.size(), std::numeric_limits<int>::max());
        // Vector to store the paths
        std::vector<std::vector<size_t>> paths(vertices.size());

        // Distance to self set to 0
        pq.push({0, src});
        dist[src] = 0;

        // Main loop of dijkstra algorithm
        while (!pq.empty())
        {
            size_t u = pq.top().second;
            pq.pop();

            // Stop when destination is reached
            if (u == dest)
                break;


            // Go through edges incident to vertex u
            for (size_t v = 0; v < incidence_matrix[u].size(); ++v)
            {
                if (incidence_matrix[u][v] != 0)
                {
                    int weight = incidence_matrix[u][v]->weight;
                    
                    // If path through u is shorter than v
                    if (dist[v] > dist[u] + weight)
                    {
                        // Update distance
                        dist[v] = dist[u] + weight;
                        // Store the path
                        paths[v] = paths[u];
                        paths[v].push_back(v);
                        pq.push({dist[v], v});
                    }
                }
            }
        }

        std::string result = "";

        // Show distances and paths
        result += "Vertex Distance from Source\n";
        result += "To id: (" + std::to_string(dest) + ")\t\tDistance: " + std::to_string(dist[dest]) + "\t\tPath: ";
        for (size_t j = 0; j < paths[dest].size(); j++)
        {
            result += std::to_string(paths[dest][j]) + " ";
        }
        result += "\n";
        return result;
    }

    // Returning graph as string in order to display it
    std::string get_as_string()
    {
        std::string output = "";
        for (size_t i = 0; i < incidence_matrix.size(); i++)
        {
            output += "Vertex " + vertices[i]->label + " (" + std::to_string(vertices[i]->id) + ") - Data: " + vertices[i]->data + "\n";
            for (size_t j = 0; j < incidence_matrix[i].size(); j++)
            {
                if (incidence_matrix[i][j] != nullptr)
                {
                    output += "  connects to Vertex " + std::to_string(j) + " with weight " + std::to_string(incidence_matrix[i][j]->weight) + "\n";
                }
            }
        }
        return output;
    }

    size_t get_size()
    {
        return vertices.size();
    }
};

#endif