#include "display_tree.h"

std::string get_node_label(Node *node) {
  if (node->token.op == Ops::NONE) {
    return node->token.prop;
  }
  // Simple conversion from Ops enum to string for the label
  switch (node->token.op) {
  case Ops::IMP:
    return "->";
  case Ops::IFF:
    return "<->";
  case Ops::AND:
    return "AND";
  case Ops::OR:
    return "OR";
  case Ops::NEG:
    return "~";
  default:
    return "Op";
  }
}

// Recursive function to print the tree in DOT format
void print_node_dot(std::ofstream &file, Node *node) {
  if (!node) {
    return;
  }

  // Use the node's memory address as a unique ID
  long long node_id = reinterpret_cast<long long>(node);

  // Define the current node's appearance
  file << "  node" << node_id << " [label=\"" << get_node_label(node)
       << "\"];\n";

  // Define connection to the left child
  if (node->left) {
    long long left_id = reinterpret_cast<long long>(node->left);
    file << "  node" << node_id << " -> node" << left_id << ";\n";
    print_node_dot(file, node->left);
  }

  // Define connection to the right child
  if (node->right) {
    long long right_id = reinterpret_cast<long long>(node->right);
    file << "  node" << node_id << " -> node" << right_id << ";\n";
    print_node_dot(file, node->right);
  }
}

void generate_graphviz_file(Solver &solver, const std::string &filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return;
  }
  Node *head = solver.tree.head;
  file << "digraph AST {\n";
  if (head) {
    print_node_dot(file, head);
  }
  file << "}\n";

  file.close();
  std::cout << "Generated Graphviz file: " << filename << std::endl;
}
