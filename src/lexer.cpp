#include "lexer.h"
#include "helpers.h"

namespace Lex {
// banning these just to enforce that the whole thing is mroe readable
const std::unordered_set<std::string> op_dic{"->", "implies", "iff", "and",
                                             "or", "~",       "(",   ")"};

const std::unordered_set<char> reserved_chars = {
    ' ', '\t', '\n', '\r', '(', ')', '$', '\\', '!', '@', '#',
    '%', '^',  '&',  '*',  '+', '=', '|', '[',  ']', '{', '}',
    ';', ':',  '\'', '"',  ',', '.', '<', '>',  '/', '?'};
} // namespace Lex

void Lexer::lex_expression() {
  std::cout << "The original expression:\n" << expression << "\n";
  std::vector<std::string> strings = split_expression(expression);
  std::cout << "Here is the expression split by spaces\n";
  for (std::string &str : strings) {
    std::cout << str << "\n";
  }

  // loop through all the strings
  // determine if it's an operation a pare or a proposition
  // then tokenize accordingly
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

// tells you if you read the expression which is the last line after this
// nothing more will be processed
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
  std::cout << "verifified prop name: " << prop_name << "\n";
  props.insert(prop_name);
}

Lexer::Lexer(const char *fileName) {
  std::ifstream file(fileName);
  if (!file.is_open()) {
    err_exit(FILE_ERR);
  }

  std::string line;
  bool processing_propositions = true;
  expression = "";

  while (std::getline(file, line)) {
    trim_white_space(line);

    if (!line.size()) {
      continue;
    }

    if (processing_propositions) {
      if (line[0] == '$') {
        processing_propositions = false;
        expression = line;
      } else {
        lex_prop(line);
      }
    } else {
      expression += " ";
      expression += line;
    }
  }

  file.close();

  if (expression.empty() || expression.back() != '$') {
    err_exit(INVALID_EXPR);
  }
  lex_expression();
}
