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
                    add_edge(i, j, weight);
                    add_edge(j, i, weight);
                }
            }
        }
    }

    Vertex<Type>* get_vertex(size_t id)
    {
        return vertices[id];
    }

    // Add edge between vertices
    void add_edge(size_t source_id, size_t destination_id, int weight)
    {
        Edge<Type>* new_edge = new Edge<Type>(vertices[source_id], vertices[destination_id], weight);
        incidence_matrix[source_id][destination_id] = new_edge;
        incidence_matrix[destination_id][source_id] = new_edge;
    }

    // Delete edge between vertices
    void remove_edge(Vertex<Type>* source, Vertex<Type>* destination)
    {
        size_t source_id = source->id;
        size_t destination_id = destination->id;
        delete incidence_matrix[source_id][destination_id];
        incidence_matrix[source_id][destination_id] = nullptr;
        delete incidence_matrix[destination_id][source_id];
        incidence_matrix[destination_id][source_id] = nullptr;
    }

    // Add a new vertex to the graph
    void add_vertex()
    {
        size_t num_vertices = vertices.size();

        // Add a new vertex
        vertices.push_back(new Vertex<Type>(Type(), "Vertex_" + std::to_string(num_vertices), num_vertices));

        // Resize each row in incidence matrix and add nullptr for the new vertex
        for (std::vector<Edge<Type>*> &row : incidence_matrix)
            row.push_back(nullptr);

        // Add a new row with nullptrs for the new vertex
        incidence_matrix.push_back(std::vector<Edge<Type>*>(num_vertices + 1, nullptr));
    }

    // Remove a vertex from the graph
    void remove_vertex(size_t vertex_id)
    {
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
                delete incidence_matrix[i][vertex_id];
                incidence_matrix[i][vertex_id] = nullptr;
            }
        }

        for(int i = vertex_id+1; i<vertices.size(); i++)
        {
            vertices[id]->id--;
        }

        // Delete the vertex object
        delete vertices[vertex_id];
        vertices.erase(vertices.begin() + vertex_id);

        // Erase the corresponding row and column from incidence matrix
        incidence_matrix.erase(incidence_matrix.begin() + vertex_id);
        for (std::vector<Edge<Type>*>& row : incidence_matrix)
            row.erase(row.begin() + vertex_id);
    }

    std::string shortest_paths(size_t src)
    {
        std::priority_queue<std::pair<int, size_t>, std::vector<std::pair<int, size_t>>, std::greater<std::pair<int, size_t>>> pq;

        // Set distances as infinity
        std::vector<int> dist(vertices_data.size(), std::numeric_limits<int>::max());
        // Vector to store the paths
        std::vector<std::vector<size_t>> paths(vertices_data.size());

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
                    int weight = incidence_matrix[u][v];
                    
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
        for (size_t i = 0; i < vertices_data.size(); i++)
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

    // Returning graph as string in order to display it
    std::string get_as_string()
    {
        std::string output = "";
        for (size_t i = 0; i < incidence_matrix.size(); i++)
        {
            output += "Vertex " + std::to_string(i) + "\n";
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
};
