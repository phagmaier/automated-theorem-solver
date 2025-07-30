#include "solver.h"

Solver::Solver(const char *fileName) {
  Lexer lex = Lexer(fileName);

  tokens = std::move(lex.tokens);
  dic = Dic(lex.props);
  tree = Tree();
}

void Solver::collect_literals(Node *node, std::vector<std::string> &clause) {
  if (!node) {
    return;
  }
  if (node->get_node_op() == Ops::OR) {
    collect_literals(node->left, clause);
    collect_literals(node->right, clause);
  } else if (node->get_node_op() == Ops::NEG) {
    std::string literal = "~" + node->left->token.prop;
    clause.push_back(literal);
  } else {
    clause.push_back(node->token.prop);
  }
}

void Solver::get_clauses(Node *node) {
  if (!node) {
    return;
  }

  if (node->get_node_op() == Ops::AND) {
    get_clauses(node->left);
    get_clauses(node->right);
  } else {
    std::vector<std::string> clause;
    collect_literals(node, clause);

    if (dic.insert(clause)) {
      clauses.push_back(clause);
    }
  }
}

void Solver::solve() {
  tree.parse(tokens);
  get_clauses(tree.head);
}
