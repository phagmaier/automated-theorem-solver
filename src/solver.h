#pragma once
#include "clause.h"
#include "lexer.h"
#include "tree.h"
#include <unordered_map>
#include <unordered_set>

struct Solver {
  Solver(const char *fileName);
  void solve();
  void negate_conclusion();
  void collect_literals(Node *node, Clause &clause);
  void get_clauses(Node *head);
  bool find_contradiction();
  bool is_empty_set(Clause &new_clause);

  DynamicBitset generate_collision(Clause &A, Clause &B);
  Clause generate_clause(Clause &A, Clause &B, DynamicBitset &collision);
  std::vector<Token> tokens;
  std::vector<Token> conclusion;
  std::vector<Token> neg_conclusion;
  int num_props;
  std::unordered_map<std::string, int> string_to_int;
  std::vector<Clause> clauses;
  std::unordered_set<Clause> clause_set;
  Tree tree;
  Tree conc_tree;
};
