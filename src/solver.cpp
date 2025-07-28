#include "solver.h"

Solver::Solver(const char *fileName) {
  Lexer lex = Lexer(fileName);
  tokens = std::move(lex.tokens);
}

void Solver::parse() { std::cerr << "PARSER ON SOLVER NOT IMPLIMENTED YET\n"; }
