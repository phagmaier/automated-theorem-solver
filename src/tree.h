
#pragma once
// #include "lexer.h"
#include "token.h"
#include <vector>

struct Node {
  Node() : token{Token()}, left{nullptr}, right{nullptr} {}
  Node(Token &token) : token{Token(token)}, left{nullptr}, right{nullptr} {}
  Node(Ops op) : token{Token(op)}, left{nullptr}, right{nullptr} {}
  Node(Token n, Node *l, Node *r) : token{Token(n)}, left{l}, right{r} {}
  Ops get_node_op() { return token.op; }
  Token token;
  Node *left;
  Node *right;
};

struct Tree {
  Tree() : head{nullptr} {}
  Tree(Token &token) : head{new Node(token)} {}
  Node *parse_factor(std::vector<Token> &tokens);
  Node *parse_conjuction(std::vector<Token> &tokens);
  Node *parse_term(std::vector<Token> &tokens);
  Node *parse_expression(std::vector<Token> &tokens);
  Ops get_cur_op() { return cur_node->get_node_op(); }
  Token &current_token(std::vector<Token> &tokens);
  void advance(int size);
  Node *clone_tree(Node *node);
  Node *eliminate_implications(Node *node);
  Node *move_negations_inward(Node *node);
  Node *distribute_or_over_and(Node *node);
  void transform_to_cnf();
  void parse(std::vector<Token> &tokens);
  int index = 0;
  Node *head;
  Node *cur_node;
};
