#include "token.h"

// enum class Ops { IMP, IFF, AND, OR, NEG, LP, RP, NONE };
void Token::print_token() {
  if (op == Ops::NONE) {
    std::cout << prop << " ";
    return;
  } else if (op == Ops::IMP) {
    std::cout << "-> ";
  } else if (op == Ops::IFF) {
    std::cout << "<-> ";
  } else if (op == Ops::AND) {
    std::cout << "&& ";
  } else if (op == Ops::OR) {
    std::cout << "|| ";
  } else if (op == Ops::NEG) {
    std::cout << "~";
  } else if (op == Ops::LP) {
    std::cout << "(";
  } else if (op == Ops::RP) {
    std::cout << ")";
  } else {
    std::cout << "\nERROR THIS SHOULD NEVER PRINT ERROR\n";
  }
}
