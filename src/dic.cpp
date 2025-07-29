#include "dic.h"

Dic::Dic() = default;

Dic::Dic(std::unordered_set<std::string> &props) {
  int i = 1;
  for (const auto &str : props) {
    string_to_int[str] = i;
    int_to_string[i] = str;
    std::string tmp = "~";
    tmp += str;
    string_to_int[tmp] = -i;
    int_to_string[-i] = tmp;
    ++i;
  }
}

bool Dic::add_clause(std::vector<std::string> &clause) {
  std::string result = "";
  std::vector<int> numeric;
  for (std::string &str : clause) {
    numeric.push_back(string_to_int[str]);
  }
  std::sort(numeric.begin(), numeric.end());
  for (int i : numeric) {
    result += int_to_string[i];
  }
  return clause_set.insert(result).second;
}

bool Dic::add_clauses(std::vector<std::vector<std::string>> &clauses) {
  bool uniique_insert = false;
  for (std::vector<std::string> &vec : clauses) {
    uniique_insert |= add_clause(vec);
  }
  return uniique_insert;
}
