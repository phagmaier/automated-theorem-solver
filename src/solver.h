#pragma once
#include "dic.h"
#include "lexer.h"
#include <vector>

struct Node {
  Node() : token{Token()}, left{nullptr}, right{nullptr} {}
  Node(Token &token) : token{Token(token)}, left{nullptr}, right{nullptr} {}
  Node(Node *n, Node *l, Node *r) : token{Token(n->token)}, left{l}, right{r} {}
  void add(Token &token);
  void add(Node *child);
  inline bool is_full() { return left && right; }
  inline bool has_children() { return left || right; }
  Ops get_node_op() { return token.op; }
  Token token;
  Node *left;
  Node *right;
};

struct Tree {
  Tree() : head{nullptr} {}
  Tree(Token &token) : head{new Node(token)} {}

  void parse(std::vector<Token> &tokens);
  Node *factor(std::vector<Token> &tokens);
  Node *parse_term(std::vector<Token> &tokens);
  Node *parse_expression(std::vector<Token> &tokens);
  void advance(std::vector<Token> &tokens);
  Ops get_cur_op() { return cur_node->get_node_op(); }
  int index = 0;
  Node *head;
  Node *cur_node;
};

struct Solver {
  Solver(const char *fileName);
  std::vector<Token> tokens;
  Dic dic;
  Tree tree;
  void solve();
};
