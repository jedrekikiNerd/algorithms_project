#include "Timer.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include "ui_actions.h"
#include <iomanip>
#include "data_structures/graph.hpp"
#include "data_structures/graph2.hpp"
#include "test_graph.hpp"

namespace fs = std::filesystem;
Timer timer2;

//clearing the folder before adding files
void removeFilesInFolder3(const std::string folderName) {
    if (!fs::exists(folderName)) {
        fs::create_directories(folderName);
    }

    for (const auto& entry : fs::directory_iterator(folderName)) {
        if (entry.is_regular_file()) {
            fs::remove(entry.path());
        }
    }
}

// Adds one line to given file
int add_line_to_file2(std::string line, std::string filePath)
{
    std::ofstream file("tests_results/"+filePath, std::ios_base::app); 
    if (file.is_open()) {
        file << line << std::endl; 
        file.close(); 
    } else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
    return 0;
}

size_t generate_random_id(size_t size_graph, std::default_random_engine& random_generator)
{
    size_t number;
    std::uniform_int_distribution<size_t> distribution(0, size_graph-1);
    number = distribution(random_generator);
    return number;
}

double dijkstra1(GraphInterface<std::string>* graph, size_t vertex_id)
{
    Timer timer2;
    timer2.start();
    graph->shortest_paths(vertex_id);
    timer2.stop();
    return timer2.elapsedSeconds();
}

double dijkstra2(GraphInterface<std::string>* graph, size_t id_from, size_t id_to)
{
    Timer timer2;
    timer2.start();
    graph->shortest_path_to(id_from, id_to);
    timer2.stop();
    return timer2.elapsedSeconds();
}

// Function that runs all tests and saves time series to files
// We use buffer (stream) because thats the solution we found to output more than 4 digits after decimal point to string
int run_all_tests_for_graphs(int data_size, int repetition)
{
    // Random generator
    std::random_device rd;
    std::default_random_engine random_generator(rd());
    GraphInterface<std::string>* graph = nullptr;
    float measure_points2[4] = {0.25, 0.5, 0.75, 1.0};
    std::ostringstream stream;
    std::string measure_line;

    // Dijkstra1 for graphs
    for (float density : measure_points2)
    {
        double temp_double_buffor = 0;
        double temp_double_buffor2 = 0;
        for(int j=0; j<repetition; j++)
        {
            graph = new DirectedWeightedGraph<std::string>;
            graph->generate_graph(data_size, density, "data");
            temp_double_buffor += dijkstra1(graph, generate_random_id(graph->get_size(), random_generator));
            temp_double_buffor2 += dijkstra2(graph, generate_random_id(graph->get_size(), random_generator), generate_random_id(graph->get_size(), random_generator));
            delete graph;
        }
        temp_double_buffor = temp_double_buffor / repetition;
        temp_double_buffor2 = temp_double_buffor2 / repetition;
        stream << std::fixed << std::setprecision(10) << temp_double_buffor;
        measure_line = std::to_string(data_size) + ";" + stream.str();
        add_line_to_file2(measure_line, ("dijkstra_to_all_g1_" + std::to_string(density) + ".txt"));
        stream.str("");
        stream << std::fixed << std::setprecision(10) << temp_double_buffor2;
        measure_line = std::to_string(data_size) + ";" + stream.str();
        add_line_to_file2(measure_line, ("dijkstra_to_chosen_g1_" + std::to_string(density) + ".txt"));
        stream.str("");

        temp_double_buffor = 0;
        temp_double_buffor2 = 0;
        for(int j=0; j<repetition; j++)
        {
            graph = new DirectedWeightedGraphIncidence<std::string>;
            graph->generate_graph(data_size, density, "data");
            temp_double_buffor += dijkstra1(graph, generate_random_id(graph->get_size(), random_generator));
            temp_double_buffor2 += dijkstra2(graph, generate_random_id(graph->get_size(), random_generator), generate_random_id(graph->get_size(), random_generator));
            delete graph;
        }
        temp_double_buffor = temp_double_buffor / repetition;
        temp_double_buffor2 = temp_double_buffor2 / repetition;
        stream << std::fixed << std::setprecision(10) << temp_double_buffor;
        measure_line = std::to_string(data_size) + ";" + stream.str();
        add_line_to_file2(measure_line, ("dijkstra_to_all_g2_" + std::to_string(density) + ".txt"));
        stream.str("");
        stream << std::fixed << std::setprecision(10) << temp_double_buffor2;
        measure_line = std::to_string(data_size) + ";" + stream.str();
        add_line_to_file2(measure_line, ("dijkstra_to_chosen_g2_" + std::to_string(density) + ".txt"));
        stream.str("");
    }

    return 0;
}

// Function that loads data from file to each data structure and runs all tests for it
int run_tests_graph()
{
    std::string confirm = user_input_action_string("Wpisz [TAK] jeżeli chcesz kontynuować: ");
    if (confirm != "TAK")
        return 0;
    int measure_points[6] = {10, 50, 100, 250, 500, 1000};

    removeFilesInFolder3("tests_results");

    int repetition = user_input_action("Podaj ile razy powtarzać pomiar: ");

    for(int i=0; i<6; i++)
    {
        std::cout << "i: " << i << "\n";
        run_all_tests_for_graphs(measure_points[i], repetition);
    }
    std::cout << "I MADE IT \n";
    return 0;
}
