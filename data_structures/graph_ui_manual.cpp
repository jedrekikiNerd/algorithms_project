#include "graph_ui_manual.hpp"
#include "../ui_actions.h"
#include "../Timer.h"
#include <iostream>
#include "graph.hpp"

// Agent between UI and graph add vertex
int add_vertex(DirectedWeightedGraph<std::string> *graph)
{
    std::string data = user_input_action_string("Podaj dane, które mają znaleźć się w wierzchołku grafu: ");
    std::string label = user_input_action_string("Podaj nazwę wierzchołka: ");
    Timer timer;
    timer.start();
    graph->add_vertex(label, data);
    timer.stop();
    if (print_time)
        display_action("Dodawanie wierzchołka zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and graph remove vertex
int remove_vertex(DirectedWeightedGraph<std::string> *graph)
{
    int id = user_input_action("Podaj id wierzchołka: ");
    Timer timer;
    timer.start();
    graph->remove_vertex(id);
    timer.stop();
    if (print_time)
        display_action("Usuwanie wierzchołka zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and graph add edge
int add_edge(DirectedWeightedGraph<std::string> *graph)
{
    int id_from = user_input_action("Podaj id wierzchołka z którego wychodzi krawędź: ");
    int id_to = user_input_action("Podaj id wierzchołka do którego wchodzi krawędź: ");
    int weight = user_input_action("Podaj wagę krawędzi: ");
    Timer timer;
    timer.start();
    graph->add_edge(id_from, id_to, double(weight));
    timer.stop();
    if (print_time)
        display_action("Dodawanie krawędzi zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and graph remove edge
int remove_edge(DirectedWeightedGraph<std::string> *graph)
{
    int id_from = user_input_action("Podaj id wierzchołka z którego wychodzi krawędź: ");
    int id_to = user_input_action("Podaj id wierzchołka do którego wchodzi krawędź: ");
    int weight = user_input_action("Podaj wagę krawędzi: ");
    Timer timer;
    timer.start();
    graph->remove_edge(id_from, id_to);
    timer.stop();
    if (print_time)
        display_action("Usuwanie krawędzi zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}
