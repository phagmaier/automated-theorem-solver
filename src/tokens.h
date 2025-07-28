#pragma once
#include "helpers.h"
struct Token {
  Token(std::string &prop) : op{Ops::NONE}, prop{prop} {}
  Token(Ops op) : op{op}, prop{""} {}
  Ops op;
  std::string prop;
};
