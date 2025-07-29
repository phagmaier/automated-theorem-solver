#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using ui = size_t;

enum class TokenType { NUM, OP, EOF_TOKEN };

enum class OperatorType { ADD, SUB, DIV, MUL, PAREN_L, PAREN_R, NONE };

struct Token {
  TokenType type;
  OperatorType op_type;
  double num_value;

  Token(OperatorType op) : type{TokenType::OP}, op_type{op}, num_value{0.0} {}

  Token(double num)
      : type{TokenType::NUM}, op_type{OperatorType::NONE}, num_value{num} {}

  Token(TokenType type)
      : type{type}, op_type{OperatorType::NONE}, num_value{0.0} {}

  void print_op_symbol() const {
    switch (op_type) {
    case OperatorType::ADD:
      std::cout << " + ";
      break;
    case OperatorType::SUB:
      std::cout << " - ";
      break;
    case OperatorType::MUL:
      std::cout << " * ";
      break;
    case OperatorType::DIV:
      std::cout << " / ";
      break;
    case OperatorType::PAREN_L:
      std::cout << " ( ";
      break;
    case OperatorType::PAREN_R:
      std::cout << " ) ";
      break;
    default:
      std::cerr << "ERROR: Invalid operator type encountered for printing."
                << std::endl;
      exit(1);
    }
  }

  void print_token_value() const {
    if (type == TokenType::OP) {
      print_op_symbol();
    } else if (type == TokenType::NUM) {
      std::cout << num_value;
    } else if (type == TokenType::EOF_TOKEN) {
      std::cout << " EOF ";
    }
  }
};

void print_expression_tokens(const std::vector<Token> &tokens) {
  std::cout << "Lexed Expression: [";
  for (const Token &token : tokens) {
    token.print_token_value();
  }
  std::cout << " ]\n";
}

void skip_white_space(const std::string &str, ui &i) {
  while (i < str.size() && std::isspace(str[i])) {
    ++i;
  }
}

bool is_digit(char c) { return std::isdigit(c); }

double parse_number(const std::string &str, ui &i) {
  std::string num_str = "";

  while (i < str.size() && is_digit(str[i])) {
    num_str += str[i];
    ++i;
  }

  if (i < str.size() && str[i] == '.') {
    num_str += str[i];
    ++i;
    while (i < str.size() && is_digit(str[i])) {
      num_str += str[i];
      ++i;
    }
  }

  if (num_str.empty() || (num_str.length() == 1 && num_str[0] == '.')) {
    throw std::runtime_error("Lexer error: Invalid number format.");
  }

  return std::stod(num_str);
}

OperatorType get_operator_type(char c) {
  switch (c) {
  case '+':
    return OperatorType::ADD;
  case '-':
    return OperatorType::SUB;
  case '*':
    return OperatorType::MUL;
  case '/':
    return OperatorType::DIV;
  case '(':
    return OperatorType::PAREN_L;
  case ')':
    return OperatorType::PAREN_R;
  default:
    throw std::runtime_error("Lexer error: Invalid operator character '" +
                             std::string(1, c) + "'");
  }
}

std::vector<Token> lex(const std::string &str) {
  std::vector<Token> tokens;
  ui i = 0;

  while (i < str.size()) {
    skip_white_space(str, i);

    if (i >= str.size()) {
      break;
    }

    char current_char = str[i];

    if (is_digit(current_char) || current_char == '.') {
      tokens.push_back(Token(parse_number(str, i)));
    } else if (current_char == '+' || current_char == '-' ||
               current_char == '*' || current_char == '/' ||
               current_char == '(' || current_char == ')') {
      tokens.push_back(Token(get_operator_type(current_char)));
      ++i;
    } else {
      throw std::runtime_error("Lexer error: Unexpected character '" +
                               std::string(1, current_char) + "' at position " +
                               std::to_string(i));
    }
  }
  tokens.push_back(Token(TokenType::EOF_TOKEN));
  return tokens;
}

class Parser {
private:
  const std::vector<Token> &tokens;
  ui current_token_index;

  const Token &current_token() const { return tokens[current_token_index]; }

  void advance() {
    if (current_token_index < tokens.size() - 1) {
      current_token_index++;
    }
  }

  void eat(TokenType type) {
    if (current_token().type == type) {
      advance();
    } else {
      throw std::runtime_error("Parser error: Expected token type " +
                               std::to_string(static_cast<int>(type)) +
                               ", but got different type at index " +
                               std::to_string(current_token_index));
    }
  }

  void eat_op(OperatorType op_type) {
    if (current_token().type == TokenType::OP &&
        current_token().op_type == op_type) {
      advance();
    } else {
      throw std::runtime_error("Parser error: Expected operator type " +
                               std::to_string(static_cast<int>(op_type)) +
                               ", but got different operator at index " +
                               std::to_string(current_token_index));
    }
  }

  double factor() {
    const Token &token = current_token();

    if (token.type == TokenType::OP && token.op_type == OperatorType::ADD) {
      eat_op(OperatorType::ADD);
      return +factor();
    } else if (token.type == TokenType::OP &&
               token.op_type == OperatorType::SUB) {
      eat_op(OperatorType::SUB);
      return -factor();
    } else if (token.type == TokenType::NUM) {
      double value = token.num_value;
      eat(TokenType::NUM);
      return value;
    } else if (token.type == TokenType::OP &&
               token.op_type == OperatorType::PAREN_L) {
      eat_op(OperatorType::PAREN_L);
      double result = expression();
      eat_op(OperatorType::PAREN_R);
      return result;
    } else {
      throw std::runtime_error("Parser error: Expected number, '(', '+', or "
                               "'-' but got unexpected token at index " +
                               std::to_string(current_token_index));
    }
  }

  double term() {
    double result = factor();

    while (current_token().type == TokenType::OP &&
           (current_token().op_type == OperatorType::MUL ||
            current_token().op_type == OperatorType::DIV)) {
      OperatorType op = current_token().op_type;
      if (op == OperatorType::MUL) {
        eat_op(OperatorType::MUL);
        result *= factor();
      } else if (op == OperatorType::DIV) {
        eat_op(OperatorType::DIV);
        double divisor = factor();
        if (divisor == 0) {
          throw std::runtime_error("Division by zero error!");
        }
        result /= divisor;
      }
    }
    return result;
  }

  double expression() {
    double result = term();

    while (current_token().type == TokenType::OP &&
           (current_token().op_type == OperatorType::ADD ||
            current_token().op_type == OperatorType::SUB)) {
      OperatorType op = current_token().op_type;
      if (op == OperatorType::ADD) {
        eat_op(OperatorType::ADD);
        result += term();
      } else if (op == OperatorType::SUB) {
        eat_op(OperatorType::SUB);
        result -= term();
      }
    }
    return result;
  }

public:
  Parser(const std::vector<Token> &tokens)
      : tokens(tokens), current_token_index(0) {}

  double parse() {
    double result = expression();
    eat(TokenType::EOF_TOKEN);
    return result;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " \"<expression>\"\n";
    std::cerr << "Example: " << argv[0] << " \"(10 + 5) * 2 - 3\"\n";
    return 1;
  }

  std::string str = argv[1];

  if (str.empty()) {
    std::cerr << "ERROR: Input expression cannot be empty.\n";
    return 1;
  }

  try {
    std::vector<Token> tokens = lex(str);
    print_expression_tokens(tokens);

    Parser parser(tokens);
    double result = parser.parse();
    std::cout << "Result: " << result << std::endl;

  } catch (const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
