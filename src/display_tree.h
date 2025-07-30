#pragma once
#include "solver.h"
#include <fstream>

std::string get_node_label(Node *node);
void print_node_dot(std::ofstream &file, Node *node);
void generate_graphviz_file(Solver &solver, const std::string &filename);
