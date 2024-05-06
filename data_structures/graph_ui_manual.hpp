#ifndef manual_ui_graph
#define manual_ui_graph

#include <iostream>
#include "Igraph.hpp"

// Agent between UI and graph add vertex
int add_vertex(GraphInterface<std::string> *graph);

// Agent between UI and graph remove vertex
int remove_vertex(GraphInterface<std::string> *graph);

// Agent between UI and graph add edge
int add_edge(GraphInterface<std::string> *graph);

// Agent between UI and graph remove edge
int remove_edge(GraphInterface<std::string> *graph);

// Agent between UI and graph generate
int generate_graph(GraphInterface<std::string> *graph);

// Agent between UI and shortest_paths
int shortest_paths(GraphInterface<std::string> *graph);

// Agent between UI and shortest_path_to
int shortest_path_to(GraphInterface<std::string> *graph);

#endif
