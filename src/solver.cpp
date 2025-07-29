#include "solver.h"
#include <vector>

Solver::Solver(const char *fileName) {
  Lexer lex = Lexer(fileName);
  tokens = std::move(lex.tokens);
  dic = Dic(lex.props);
  tree = Tree();
}

void Node::add(Token &token) {
  if (left) {
    left = new Node(token);
  } else {
    right = new Node(token);
  }
}

void Node::add(Node *child) {
  if (left) {
    left = child;
  } else {
    right = child;
  }
}

void Tree::advance(std::vector<Token> &tokens) {
  cur_node = new Node(tokens[index++]);
}

// for -> and //<->
Node *Tree::parse_expression(std::vector<Token> &tokens) {
  // 1. Get the left-hand side by calling the next function in the precedence
  // chain.
  Node *node = parse_term(tokens);

  // 2. Look for lowest precedence operators (IMPLIES, IFF).
  while (index < tokens.size() &&
         (get_cur_op() == Ops::IMP || get_cur_op() == Ops::IFF)) {

    Node *op_node = cur_node;
    advance(tokens); // Consume the operator token (e.g., '->').

    // 3. Get the right-hand side.
    Node *right_node = parse_term(tokens);

    // 4. Create a new parent node, with the previous tree as the left child.
    node = new Node(op_node, node, right_node);
  }

  return node;
}
