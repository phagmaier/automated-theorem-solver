#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

/*RULES FOR INPUT
 * prop -> declares a prop
 * One decleration per line (as many blank lines in between though)
 * For now no comments
 * / before all ops (except parenthesies)
 * Must be a space between everything (including parens)
 * Statment must be enclosed by $$
 * We can only evaluate one expression at a time
 * */

/*Precedence	Operator	Name
    Highest	( )	Parentheses
    ∀,∃	Quantifiers
    ¬	Negation
    ∧	Conjunction
    ∨	Disjunction
    Lowest	→,↔	Implication, Biconditional
*/

enum class Type { PROP, OP, ERROR };
enum class Ops { IMP, IFF, AND, OR, NEG, LP, RP, NONE };

enum MyErrs {
  NO_ARGS,
  MANY_ARGS,
  FILE_ERR,
  RESERVED_CHAR,
  EMPTY_PROP,
  INVALID_STATMENT,
  DUPLICATE_PROP
};

void err_exit(MyErrs e) {
  switch (e) {
  case NO_ARGS:
    throw std::runtime_error("NO ARGUMENTS PROVIDED EXPECTED FILE NAME");
  case MANY_ARGS:
    throw std::runtime_error("TOO FEW ARGUMENTS PROVIDED");
  case RESERVED_CHAR:
    throw std::runtime_error("USED A RESERVED CHAR");
  case EMPTY_PROP:
    throw std::runtime_error(
        "EMPTY PROPOSITION. PROP KEYWORD USED BUT WAS NOT SET");
  case INVALID_STATMENT:
    throw std::runtime_error("INVALID STATMENT");
  case DUPLICATE_PROP:
    throw std::runtime_error("INVALID STATMENT");

  default:
    std::cerr << "SOMETHING BROKE\n";
    throw std::runtime_error("ALSO YOU PROVIDED AN INVALID ERROR CODE");
  }
}

struct Token {
  Token(std::string &prop);
  Token(Ops op);
  Ops op;
  std::string prop;
};

Token::Token(std::string &prop) : op{Ops::NONE}, prop{prop} {}

Token::Token(Ops op) : op{op}, prop{""} {}

struct Lexer {
  Lexer(const char *fileName);
  void read_file(const char *fileName);
  std::vector<Token> contents;
  std::unordered_set<std::string, int> valid_props;
  void parse_prop(std::string &str);
  void parse_line(std::string &str);
  void is_valid_prop(const std::string &str);
  void parse_statment(std::string &string);
};

void Lexer::is_valid_prop(const std::string &str) {

  static const std::unordered_set<char> reserved_chars = {
      '\t', '\n', '\r', '(', ')', '$', '\\', '!', '@', '#', '%',
      '^',  '&',  '*',  '+', '=', '|', '[',  ']', '{', '}', ';',
      ':',  '\'', '"',  ',', '.', '<', '>',  '/', '?'};

  if (!str.size()) {
    err_exit(EMPTY_PROP);
  }

  for (const char c : str) {
    if (reserved_chars.count(c) > 0) {
      std::cerr << "CANNOT USE THE CHAR " << c << " ";
      err_exit(RESERVED_CHAR);
    }
  }
  if (valid_props.count(str) > 0) {
    err_exit(DUPLICATE_PROP);
  }
  valid_props.insert(str);
}
void Lexer::parse_prop(std::string &str) {
  static const char *prefix = "prop ";
  static const int prefix_size = 5;
  const int str_size = str.size();
  if (str.size() < prefix_size) {
    err_exit(INVALID_STATMENT);
  }
  int idx = 0;
  while (idx < prefix_size) {
    if (str[idx] != prefix[idx]) {
      err_exit(INVALID_STATMENT);
    }
    ++idx;
  }
  std::string name = "";
  while (idx < str_size) {
    name += str[idx];
    if (str[idx] == ' ') {
      ++idx;
      while (idx < str_size) {
        if (str[idx++] != ' ') {
          err_exit(INVALID_STATMENT);
        }
      }
    }
    ++idx;
  }
  is_valid_prop(name);
}

void Lexer::parse_statment(std::string &str) {}

void Lexer::parse_line(std::string &str) {
  if (!str.size()) {
    return;
  }
  if (str[0] == '$') {
    parse_statment(str);
    return;
  }
  if (str[0] == 'p') {
    parse_prop(str);
    return;
  }
  if (str[0] == ' ') {
    int i = 1;
    while (i < str.size() && str[i] == ' ') {
      ++i;
    }
    std::string tmp = str.substr(i, str.size() - i);
    return parse_line(tmp);
  } else {
    err_exit(INVALID_STATMENT);
  }
}

void Lexer::read_file(const char *fileName) {
  std::ifstream inputFile("your_file.txt");
  if (!inputFile.is_open()) {
    err_exit(FILE_ERR);
    return;
  }

  std::string line;

  while (std::getline(inputFile, line)) {
    parse_line(line);
    // buffer << line << " ";
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "TOO MANY ARGS";
    return 1;
  }
  if (argc > 2) {
    err_exit(MANY_ARGS);
  }

  return 0;
}
