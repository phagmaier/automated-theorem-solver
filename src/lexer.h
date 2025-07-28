#pragma once
#include "helpers.h"
#include "tokens.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

struct Lexer {
  Lexer(const char *fileName);
  void lex_prop(std::string &str);
  void lex_expression();
  void is_prop_valid(std::string &str);
  std::vector<Token> tokens;
  std::string expression;
  std::unordered_set<std::string> props;
};
