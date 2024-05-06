#include "graph_ui_manual.hpp"
#include "../ui_actions.h"
#include "../Timer.hpp"
#include <iostream>
#include "Igraph.hpp"

// Agent between UI and graph add vertex
int add_vertex(GraphInterface<std::string> *graph)
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
int remove_vertex(GraphInterface<std::string> *graph)
{
    size_t id = user_input_action("Podaj id wierzchołka: ");
    Timer timer;
    timer.start();
    graph->remove_vertex(graph->get_vertex(id));
    timer.stop();
    if (print_time)
        display_action("Usuwanie wierzchołka zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and graph add edge
int add_edge(GraphInterface<std::string> *graph)
{
    size_t id_from = user_input_action("Podaj id wierzchołka z którego wychodzi krawędź: ");
    size_t id_to = user_input_action("Podaj id wierzchołka do którego wchodzi krawędź: ");
    int weight = user_input_action("Podaj wagę krawędzi: ");
    Timer timer;
    timer.start();
    graph->add_edge(graph->get_vertex(id_from), graph->get_vertex(id_to), weight);
    timer.stop();
    if (print_time)
        display_action("Dodawanie krawędzi zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and graph remove edge
int remove_edge(GraphInterface<std::string> *graph)
{
    size_t id_from = user_input_action("Podaj id wierzchołka z którego wychodzi krawędź: ");
    size_t id_to = user_input_action("Podaj id wierzchołka do którego wchodzi krawędź: ");
    Timer timer;
    timer.start();
    graph->remove_edge(graph->get_vertex(id_from), graph->get_vertex(id_to));
    timer.stop();
    if (print_time)
        display_action("Usuwanie krawędzi zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and graph generate
int generate_graph(GraphInterface<std::string> *graph)
{
    int num_vertices = user_input_action("Podaj ilość wierzchołków: ");
    float density = user_input_action_float("Podaj gęstość [0,0-1,0]: ");
    std::string default_data = user_input_action_string("Podaj domyślne dane wpisywane do wierzchołków (string): ");
    Timer timer;
    timer.start();
    graph->generate_graph(num_vertices, density, default_data);
    timer.stop();
    if (print_time)
        display_action("Generowanie grafu zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and shortest_paths
int shortest_paths(GraphInterface<std::string> *graph)
{
    size_t vertex_id = user_input_action("Podaj identyfikator wierzchołka: ");
    Timer timer;
    timer.start();
    std::string result = graph->shortest_paths(vertex_id);
    timer.stop();
    if (print_time)
    {
        display_action_special(result);
        display_action("Znalazeienie wszystkich ścieżek z podanego wierzchołka zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    }
    else
        display_action_special(result);
    return 0;
}

// Agent between UI and shortest_path_to
int shortest_path_to(GraphInterface<std::string> *graph)
{
    size_t vertex_id = user_input_action("Podaj identyfikator wierzchołka źródłowego: ");
    size_t vertex_dest_id = user_input_action("Podaj identyfikator wierzchołka docelowego: ");
    Timer timer;
    timer.start();
    std::string result = graph->shortest_path_to(vertex_id, vertex_dest_id);
    timer.stop();
    if (print_time)
    {
        display_action_special(result);
        display_action("Znalazeienie wszystkich ścieżki z podanego wierzchołka zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    }
    else
        display_action_special(result);
    return 0;
}