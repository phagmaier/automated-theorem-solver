#pragma once
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
enum class Type { PROP, OP, ERROR };
enum class Ops { IMP, IFF, AND, OR, NEG, LP, RP, NONE };

enum MyErrs {
  NO_ARGS,
  MANY_ARGS,
  FILE_ERR,
  RESERVED_SYMBOL,
  EMPTY_PROP,
  INVALID_STATMENT,
  INVALID_EXPR,
  DUPLICATE_PROP
};

void err_exit(MyErrs e);

void trim_white_space(std::string &str);

int skip_whitespace(std::string &str, int i);
std::vector<std::string> split_expression(std::string &str);
