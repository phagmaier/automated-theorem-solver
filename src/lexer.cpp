#include "lexer.h"
#include "helpers.h"
#include "token.h"
#include <unordered_map>

namespace Lex {
const std::unordered_set<std::string> op_dic{
    "->", "implies", "IMPLIES", "iff", "IFF", "<->", "and", "AND",
    "&&", "or",      "OR",      "||",  "~",   "NEG", "(",   ")"};

std::unordered_map<std::string, Ops> string_to_op = {
    {"->", Ops::IMP},  {"implies", Ops::IMP}, {"IMPLIES", Ops::IMP},
    {"iff", Ops::IFF}, {"IFF", Ops::IFF},     {"<->", Ops::IFF},
    {"and", Ops::AND}, {"AND", Ops::AND},     {"&&", Ops::AND},
    {"or", Ops::OR},   {"OR", Ops::OR},       {"||", Ops::OR},
    {"~", Ops::NEG},   {"NEG", Ops::NEG},     {"(", Ops::LP},
    {")", Ops::RP}};

const std::unordered_set<char> reserved_chars = {
    ' ', '\t', '\n', '\r', '(', ')', '$', '\\', '!', '@', '#',
    '%', '^',  '&',  '*',  '+', '=', '|', '[',  ']', '{', '}',
    ';', ':',  '\'', '"',  ',', '.', '<', '>',  '/', '?', '~'};
} // namespace Lex

void Lexer::print_tokens() {
  for (Token &token : tokens) {
    token.print_token();
  }
  std::cout << "CONCLUSION:\n";
  for (Token &token : conclusion_tokens) {
    token.print_token();
  }
  // conclusion_token.print_token();
  std::cout << "\n";
}

void Lexer::tokenize_symbol(std::string &str) {
  if (Lex::op_dic.count(str)) {
    tokens.emplace_back(Token(Lex::string_to_op[str]));
  } else if (props.count(str)) {
    tokens.emplace_back(Token(str));
  } else {
    std::cerr << "COULD NOT PARSE: " << str << "\n";
    err_exit(INVALID_EXPR);
  }
}

void Lexer::lex_expression() {
  // std::cout << "The original expression:\n" << expression << "\n";
  std::vector<std::string> strings = split_expression(expression);
  // std::cout << "Here is the expression split by spaces\n";
  /*
  for (std::string &str : strings) {
    std::cout << str << "\n";
  }
  */
  for (std::string &str : strings) {
    tokenize_symbol(str);
  }
}

void Lexer::is_prop_valid(std::string &str) {
  if (props.count(str)) {
    err_exit(DUPLICATE_PROP);
  }
  if (Lex::op_dic.count(str)) {
    err_exit(RESERVED_SYMBOL);
  }
  for (char c : str) {
    if (Lex::reserved_chars.count(c)) {
      err_exit(RESERVED_SYMBOL);
    }
  }
}

void Lexer::lex_prop(std::string &str) {
  static constexpr const char *prop_type = "prop ";
  static constexpr int prop_type_size = 5;
  if (str.size() < prop_type_size + 1 &&
      str.compare(0, prop_type_size, prop_type)) {
    err_exit(INVALID_EXPR);
  }
  std::string prop_name =
      str.substr(prop_type_size, str.size() - prop_type_size);
  trim_white_space(prop_name);
  is_prop_valid(prop_name);
  // std::cout << "INSERTING " << str << " INTO VALLID PROPS\n";
  props.insert(prop_name);
}

void Lexer::lex_conclusion() {
  if (!conclusion.size()) {
    err_exit(NO_CONCLUSION);
  }
  std::vector<std::string> strings = split_expression(conclusion);
  if (!conclusion.size()) {
    err_exit(NO_CONCLUSION);
  }
  for (std::string &str : strings) {
    if (Lex::op_dic.count(str)) {
      conclusion_tokens.emplace_back(Token(Lex::string_to_op[str]));
    } else if (props.count(str)) {
      conclusion_tokens.emplace_back(Token(str));
    } else {
      std::cerr << "COULD NOT PARSE: " << str << "\n";
      err_exit(INVALID_CONC);
    }
  }
}

void Lexer::read_conclusion(std::ifstream &file) {
  std::string line;
  while (std::getline(file, line)) {
    trim_white_space(line);
    if (line.size()) {
      if (conclusion.size()) {
        conclusion += ' ';
      }
      conclusion += line;
    }
  }
}

void Lexer::read_expression(std::ifstream &file) {
  std::string line;
  while (std::getline(file, line)) {
    trim_white_space(line);
    if (line.empty()) {
      continue;
    }

    if (line.back() == '$') {
      line.pop_back();
      if (!line.empty()) {
        expression += " " + line;
      }
      read_conclusion(file);
      return;
    } else {
      expression += " " + line;
    }
  }
  err_exit(NO_CONCLUSION);
}

void Lexer::read_propositions(std::ifstream &file) {
  std::string line;
  while (std::getline(file, line)) {
    trim_white_space(line);
    if (line.empty()) {
      continue;
    }

    if (line.rfind("prop ", 0) == 0) {
      lex_prop(line);
    } else if (line.front() == '$') {
      if (line.back() == '$') {
        expression = line.substr(1, line.length() - 2);
        read_conclusion(file);
        return;
      } else {
        expression = line.substr(1);
        read_expression(file);
        return;
      }
    }
  }
  err_exit(NO_EXPR);
}

Lexer::Lexer(const char *fileName) {
  std::ifstream file(fileName);
  if (!file.is_open()) {
    err_exit(FILE_ERR);
  }

  read_propositions(file);
  file.close();

  lex_expression();
  lex_conclusion();

  std::cout << "PRINTING THE TOKENIZED EXPRESSION\n";
  print_tokens();
}
