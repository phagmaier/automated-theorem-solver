#include "tree.h"

Token &Tree::current_token(std::vector<Token> &tokens) {
  if (index >= tokens.size()) {
    throw std::runtime_error("Syntax Error: Unexpected end of expression.");
  }
  return tokens[index];
}

void Tree::advance(int size) {
  if (index < size) {
    index++;
  }
}

Node *Tree::parse_factor(std::vector<Token> &tokens) {
  Token &token = current_token(tokens);

  if (token.op == Ops::NONE && !token.prop.empty()) {
    advance(tokens.size());
    return new Node(token);
  } else if (token.op == Ops::NEG) {
    advance(tokens.size());
    Node *operand = parse_factor(tokens);
    return new Node(token, operand, nullptr);
  } else if (token.op == Ops::LP) {
    advance(tokens.size());
    Node *expression_node = parse_expression(tokens);

    if (index >= tokens.size() || current_token(tokens).op != Ops::RP) {
      throw std::runtime_error("Syntax Error: Mismatched parentheses.");
    }
    advance(tokens.size()); // Consume ')'
    return expression_node;
  } else {
    throw std::runtime_error("Syntax Error: Unexpected token.");
  }
}

Node *Tree::parse_conjuction(std::vector<Token> &tokens) {
  Node *node = parse_factor(tokens);
  while (index < tokens.size() && current_token(tokens).op == Ops::AND) {
    Token op_token = current_token(tokens);
    advance(tokens.size());
    Node *right_node = parse_factor(tokens);
    node = new Node(op_token, node, right_node);
  }
  return node;
}

Node *Tree::parse_term(std::vector<Token> &tokens) {
  Node *node = parse_conjuction(tokens);
  while (index < tokens.size() && current_token(tokens).op == Ops::OR) {
    Token op_token = current_token(tokens);
    advance(tokens.size());
    Node *right_node = parse_conjuction(tokens);
    node = new Node(op_token, node, right_node);
  }
  return node;
}

Node *Tree::parse_expression(std::vector<Token> &tokens) {
  Node *node = parse_term(tokens);
  while (index < tokens.size() && (current_token(tokens).op == Ops::IMP ||
                                   current_token(tokens).op == Ops::IFF)) {
    Token op_token = current_token(tokens);
    advance(tokens.size());
    Node *right_node = parse_term(tokens);
    node = new Node(op_token, node, right_node);
  }
  return node;
}

// --- CNF Transformation Functions ---

Node *Tree::clone_tree(Node *node) {
  if (!node) {
    return nullptr;
  }
  Node *new_node = new Node(node->token);
  new_node->left = clone_tree(node->left);
  new_node->right = clone_tree(node->right);
  return new_node;
}

Node *Tree::eliminate_implications(Node *node) {
  if (!node) {
    return nullptr;
  }

  node->left = eliminate_implications(node->left);
  node->right = eliminate_implications(node->right);

  if (node->token.op == Ops::IMP) {
    Node *not_A = new Node(Token(Ops::NEG), node->left, nullptr);
    Node *or_node = new Node(Token(Ops::OR), not_A, node->right);
    node->left = nullptr;
    node->right = nullptr;
    delete node;
    return or_node;
  } else if (node->token.op == Ops::IFF) {
    Node *a_copy1 = clone_tree(node->left);
    Node *b_copy1 = clone_tree(node->right);
    Node *a_copy2 = clone_tree(node->left);
    Node *b_copy2 = clone_tree(node->right);

    Node *imp1 = new Node(Token(Ops::IMP), a_copy1, b_copy1);
    Node *imp2 = new Node(Token(Ops::IMP), b_copy2, a_copy2);
    Node *and_node = new Node(Token(Ops::AND), imp1, imp2);

    delete node;
    return eliminate_implications(and_node);
  }

  return node;
}

Node *Tree::move_negations_inward(Node *node) {
  if (!node) {
    return nullptr;
  }

  if (node->token.op != Ops::NEG) {
    node->left = move_negations_inward(node->left);
    node->right = move_negations_inward(node->right);
    return node;
  }

  Node *child = node->left;

  if (child->token.op == Ops::NEG) {
    Node *grandchild = child->left;
    child->left = nullptr;
    delete node;
    delete child;
    return move_negations_inward(grandchild);
  } else if (child->token.op == Ops::AND) {
    Node *new_or = new Node(Ops::OR);
    new_or->left = new Node(Token(Ops::NEG), child->left, nullptr);
    new_or->right = new Node(Token(Ops::NEG), child->right, nullptr);
    child->left = nullptr;
    child->right = nullptr;
    delete node;
    delete child;
    return move_negations_inward(new_or);
  } else if (child->token.op == Ops::OR) {
    Node *new_and = new Node(Ops::AND);
    new_and->left = new Node(Token(Ops::NEG), child->left, nullptr);
    new_and->right = new Node(Token(Ops::NEG), child->right, nullptr);
    child->left = nullptr;
    child->right = nullptr;
    delete node;
    delete child;
    return move_negations_inward(new_and);
  }

  return node;
}

// Replace your existing function with this one
Node *Tree::distribute_or_over_and(Node *node) {
  if (!node) {
    return nullptr;
  }

  node->left = distribute_or_over_and(node->left);
  node->right = distribute_or_over_and(node->right);

  if (node->token.op != Ops::OR) {
    return node;
  }

  // Case 1: A v (B ^ C)
  if (node->right && node->right->token.op == Ops::AND) {
    Node *a = node->left;
    Node *b = node->right->left;
    Node *c = node->right->right;

    // Create the two new OR branches: (A v B) and (A v C)
    Node *new_or1 = new Node(Token(Ops::OR), a, b);
    Node *new_or2 = new Node(Token(Ops::OR), clone_tree(a), c);

    // Create the new top-level AND
    Node *new_and = new Node(Ops::AND);

    // IMPORTANT: Recursively apply distribution on the new branches
    new_and->left = distribute_or_over_and(new_or1);
    new_and->right = distribute_or_over_and(new_or2);

    // Clean up the now-empty old AND node shell
    node->right->left = nullptr;
    node->right->right = nullptr;
    delete node->right;   // Delete old AND
    node->left = nullptr; // Disconnect child A before deleting its parent
    delete node;          // Delete old OR

    return new_and;
  }
  // Case 2: (A ^ B) v C
  else if (node->left && node->left->token.op == Ops::AND) {
    Node *a = node->left->left;
    Node *b = node->left->right;
    Node *c = node->right;

    // Create the two new OR branches: (A v C) and (B v C)
    Node *new_or1 = new Node(Token(Ops::OR), a, c);
    Node *new_or2 = new Node(Token(Ops::OR), b, clone_tree(c));

    // Create the new top-level AND
    Node *new_and = new Node(Ops::AND);

    // IMPORTANT: Recursively apply distribution on the new branches
    new_and->left = distribute_or_over_and(new_or1);
    new_and->right = distribute_or_over_and(new_or2);

    // Clean up
    node->left->left = nullptr;
    node->left->right = nullptr;
    delete node->left;     // Delete old AND
    node->right = nullptr; // Disconnect child C before deleting its parent
    delete node;           // Delete old OR

    return new_and;
  }

  return node;
}
void Tree::transform_to_cnf() {
  if (!head)
    return;
  head = eliminate_implications(head);
  head = move_negations_inward(head);
  head = distribute_or_over_and(head);
}

void Tree::parse(std::vector<Token> &tokens) {
  index = 0;
  if (tokens.empty()) {
    head = nullptr;
  } else {
    head = parse_expression(tokens);
  }

  if (index < tokens.size()) {
    throw std::runtime_error(
        "Syntax Error: Extra tokens at end of expression.");
  }
  transform_to_cnf();
}
