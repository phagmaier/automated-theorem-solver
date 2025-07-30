#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
struct Dic {
  Dic(std::unordered_set<std::string> &props);
  Dic();
  bool insert(std::vector<std::string> &clause);
  bool insert(std::vector<std::vector<std::string>> &clauses);
  std::unordered_map<std::string, int> string_to_int;
  std::unordered_map<int, std::string> int_to_string;
  std::unordered_set<std::string> clause_set;
};
