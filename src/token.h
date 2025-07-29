#pragma once
#include "helpers.h"
struct Token {
  Token(std::string &prop) : op{Ops::NONE}, prop{prop} {}
  Token(Ops op) : op{op}, prop{""} {}
  Token(Token &token) : op{token.op}, prop{token.prop} {}
  Token() : op(Ops::NONE), prop{""} {}
  Ops op;
  std::string prop;
  void print_token();
};
