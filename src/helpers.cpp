#include "helpers.h"
#include <cctype>
void err_exit(MyErrs e) {
  switch (e) {
  case NO_ARGS:
    throw std::runtime_error("NO ARGUMENTS PROVIDED EXPECTED FILE NAME");
  case MANY_ARGS:
    throw std::runtime_error("TOO FEW ARGUMENTS PROVIDED");
  case RESERVED_SYMBOL:
    throw std::runtime_error("USED A RESERVED SYMBOL");
  case EMPTY_PROP:
    throw std::runtime_error(
        "EMPTY PROPOSITION. PROP KEYWORD USED BUT WAS NOT SET");
  case INVALID_STATMENT:
    throw std::runtime_error("INVALID STATMENT");
  case INVALID_EXPR:
    throw std::runtime_error("INVALID EXPRESSION");
  case NO_EXPR:
    throw std::runtime_error("NO STATMENT");
  case NO_CONCLUSION:
    throw std::runtime_error("NO CONCLUSION");
  case INVALID_CONC:
    throw std::runtime_error("INVALID CONCLUSION");

  case DUPLICATE_PROP:
    throw std::runtime_error("INVALID STATMENT");

  default:
    std::cerr << "SOMETHING BROKE\n";
    throw std::runtime_error("ALSO YOU PROVIDED AN INVALID ERROR CODE");
  }
}

void trim_white_space(std::string &str) {
  str.erase(str.begin(),
            std::find_if_not(str.begin(), str.end(),
                             [](unsigned char c) { return std::isspace(c); }));
  str.erase(std::find_if_not(str.rbegin(), str.rend(),
                             [](unsigned char c) { return std::isspace(c); })
                .base(),
            str.end());
}

int skip_whitespace(std::string &str, int i) {
  while (i < str.size() && std::isspace(str[i])) {
    ++i;
  }
  return i;
}

std::vector<std::string> split_expression(std::string &str) {
  std::string word = "";
  std::vector<std::string> words;
  for (char c : str) {
    if (std::isspace(static_cast<unsigned char>(c))) {
      if (!word.empty()) {
        words.push_back(word);
        word.clear();
      }
    } else if (c == '(' || c == ')' || c == '~') {
      if (!word.empty()) {
        words.push_back(word);
        word.clear();
      }
      words.push_back(std::string(1, c));
    } else {
      word += c;
    }
  }
  if (!word.empty()) {
    words.push_back(word);
  }
  return words;
}
