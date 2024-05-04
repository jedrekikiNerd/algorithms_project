#ifndef manual_ui_graph
#define manual_ui_graph

#include <iostream>
#include "graph.hpp"

// Agent between UI and graph add vertex
int add_vertex(DirectedWeightedGraph<std::string> *graph);

// Agent between UI and graph remove vertex
int remove_vertex(DirectedWeightedGraph<std::string> *graph);

// Agent between UI and graph add edge
int add_edge(DirectedWeightedGraph<std::string> *graph);

// Agent between UI and graph remove edge
int remove_edge(DirectedWeightedGraph<std::string> *graph);

#endif
