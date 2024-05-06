#include <iostream>
#include <vector>
#include <random>

template <typename Type>
class DirectedWeightedGraphIncidence
{
public:
    std::vector<std::vector<int>> incidence_matrix;

    DirectedWeightedGraph() {}

    void generate_graph(int num_vertices, float density, Type default_data)
    {
        // Reset incidence matrix
        incidence_matrix.clear();

        // Add given number of vertices
        for (int i = 0; i < num_vertices; ++i)
        {
            std::vector<int> row(num_vertices, 0); // Initialize row with zeros
            incidence_matrix.push_back(row);
        }

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

    // Add edge between vertices
    void add_edge(size_t source_id, size_t destination_id, int weight)
    {
        incidence_matrix[source_id][destination_id] = weight;
        incidence_matrix[destination_id][source_id] = weight;
    }

    // Delete edge between vertices
    void remove_edge(size_t source_id, size_t destination_id)
    {
        incidence_matrix[source_id][destination_id] = 0;
        incidence_matrix[destination_id][source_id] = 0;
    }

    // Returning graph as string in order to display it
    std::string get_as_string()
    {
        std::string output = "";
        for (size_t i = 0; i < incidence_matrix.size(); ++i)
        {
            output += "Vertex " + std::to_string(i) + "\n";
            for (size_t j = 0; j < incidence_matrix[i].size(); ++j)
            {
                if (incidence_matrix[i][j] != 0)
                {
                    output += "  connects to Vertex " + std::to_string(j) + " with weight " + std::to_string(incidence_matrix[i][j]) + "\n";
                }
            }
        }
        return output;
    }
};
