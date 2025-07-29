#pragma once
#include "token.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
struct Dic {
  Dic(std::unordered_set<std::string> &props);
  Dic();
  bool add_clause(std::vector<std::string> &clause);
  bool add_clauses(std::vector<std::vector<std::string>> &clauses);
  std::unordered_map<std::string, int> string_to_int;
  std::unordered_map<int, std::string> int_to_string;
  std::unordered_set<std::string> clause_set;
};
