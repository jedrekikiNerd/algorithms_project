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
Timer timer;

//clearing the folder before adding files
void removeFilesInFolder2(const std::string folderName) {
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
int add_line_to_file(std::string line, std::string filePath)
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

double dijkstra1_g1(DirectedWeightedGraph<std::string>* graph)
{
    Timer timer;
    timer.start();
    graph->shortest_paths(//add arguments);
    timer.stop();
    return timer.elapsedSeconds();
}

double dijkstra2_g1(DirectedWeightedGraph<std::string>* graph)
{
    Timer timer;
    timer.start();
    graph->shortest_path_to(//add arguments);
    timer.stop();
    return timer.elapsedSeconds();
}

double dijkstra1_g2(DirectedWeightedGraph<std::string>* graph)
{
    Timer timer;
    timer.start();
    graph->shortest_paths(//add arguments);
    timer.stop();
    return timer.elapsedSeconds();
}

double dijkstra2_g2(DirectedWeightedGraph<std::string>* graph)
{
    Timer timer;
    timer.start();
    graph->shortest_path_to(//add arguments);
    timer.stop();
    return timer.elapsedSeconds();
}

// Function that runs all tests and saves time series to files
// We use buffer (stream) because thats the solution we found to output more than 4 digits after decimal point to string
int run_all_tests_for_graphs(int data_size, int repetition)
{
    DirectedWeightedGraph<std::string>* graph = nullptr;
    DirectedWeightedGraph<std::string>* graph2 = nullptr;
    float measure_points2[4] = {0.25, 0.5, 0.75, 1.0};
    std::ostringstream stream;
    std::string measure_line;

    // Dijkstra1 for graph1
    for (float density : measure_points2)
    {
        double temp_double_buffor = 0;
        for(int j=0; j<repetition; j++)
        {
            temp_double_buffor += dijkstra1_g1(graph);
        }
        stream << std::fixed << std::setprecision(10) << temp_double_buffor;
        measure_line = std::to_string(data_size) + ";" + stream.str();
        add_line_to_file(measure_line, ("dijkstra_to_all" + std::to_string(density) + ".txt"));
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
    int measure_points[5] = {10, 50, 100, 500, 1000};

    removeFilesInFolder2("tests_results");

    int repetition = user_input_action("Podaj ile razy powtarzać pomiar: ");

        for(int i=0; i<5; i++)
        {
            run_all_tests_for_graphs(measure_points[i], repetition);
        }
    return 0;
}
