#pragma once
#include "lexer.h"

struct Solver {
  Solver(const char *fileName);
  std::vector<Token> tokens;
  void parse();
};
