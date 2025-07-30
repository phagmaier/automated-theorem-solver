#pragma once
#include "clause.h"
#include "lexer.h"
#include "tree.h"

struct Solver {
  Solver(const char *fileName);
  void solve();
  void collect_literals(Node *node, std::vector<std::string> &clause);
  void get_clauses(Node *head);
  std::vector<Token> tokens;
  std::vector<std::vector<std::string>> clauses;
  std::unordered_map<Clause> clause_dic;
  Tree tree;
};
