#pragma once
#include "helpers.h"
#include "token.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Lexer {
  Lexer(const char *fileName);
  void read_propositions(std::ifstream &file);
  void read_expression(std::ifstream &file);
  void read_conclusion(std::ifstream &file);
  void lex_prop(std::string &str);
  void lex_expression();
  void lex_conclusion();
  void is_prop_valid(std::string &str);
  void tokenize_symbol(std::string &str);
  void print_tokens();

  std::vector<Token> tokens;
  std::string expression;
  std::string conclusion;
  std::vector<Token> conclusion_tokens;
  std::unordered_set<std::string> props;
};
