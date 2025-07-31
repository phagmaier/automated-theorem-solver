#include "solver.h"

Solver::Solver(const char *fileName) {
  Lexer lex = Lexer(fileName);

  tokens = std::move(lex.tokens);
  conclusion = std::move(lex.conclusion_tokens);
  negate_conclusion();
  num_props = 0;
  for (auto &token : lex.props) {
    string_to_int[token] = num_props++;
  }
  tree = Tree();
  conc_tree = Tree();
}

void Solver::collect_literals(Node *node, Clause &clause) {
  if (!node) {
    return;
  }
  if (node->get_node_op() == Ops::OR) {
    collect_literals(node->left, clause);
    collect_literals(node->right, clause);
  } else if (node->get_node_op() == Ops::NEG) {
    clause.set_negative(string_to_int[node->token.prop]);
  } else {
    clause.set_positive(string_to_int[node->token.prop]);
  }
}

void Solver::get_clauses(Node *node) {
  if (!node) {
    return;
  }

  if (node->get_node_op() == Ops::AND) {
    get_clauses(node->left);
    get_clauses(node->right);
  } else {
    Clause clause = Clause(num_props);
    collect_literals(node, clause);

    if (clause_set.insert(clause).second) {
      clauses.push_back(clause);
    }
  }
}

bool Solver::is_empty_set(Clause &new_clause) {
  return (new_clause.positive_mask.is_zero() &&
          new_clause.negative_mask.is_zero());
}

// if clash.count() == 1 then you generate new clause
DynamicBitset Solver::generate_collision(Clause &A, Clause &B) {
  return (A.positive_mask & B.negative_mask) |
         (A.negative_mask & B.positive_mask);
}
Clause Solver::generate_clause(Clause &A, Clause &B, DynamicBitset &collision) {
  Clause new_clause = Clause(num_props);
  new_clause.positive_mask = A.positive_mask | B.positive_mask;
  new_clause.negative_mask = A.negative_mask | B.negative_mask;
  new_clause.positive_mask &= ~collision;
  new_clause.negative_mask &= ~collision;
  return new_clause;
}

// In solver.cpp
bool Solver::find_contradiction() {
  bool new_clause_found_in_pass;
  do {
    new_clause_found_in_pass = false;

    int clauses_to_process = clauses.size();

    for (int i = 0; i < clauses_to_process; ++i) {
      for (int j = i + 1; j < clauses_to_process; ++j) {
        Clause &A = clauses[i];
        Clause &B = clauses[j];

        DynamicBitset collision = generate_collision(A, B);

        if (collision.count() == 1) {
          Clause new_clause = generate_clause(A, B, collision);

          if (is_empty_set(new_clause)) {
            return true;
          }
          if (clause_set.insert(new_clause).second) {
            clauses.push_back(new_clause);
            new_clause_found_in_pass = true;
          }
        }
      }
    }
  } while (new_clause_found_in_pass);

  return false;
}
void Solver::negate_conclusion() {
  neg_conclusion.push_back(Token(Ops::NEG));
  neg_conclusion.push_back(Token(Ops::LP));
  for (Token &token : conclusion) {
    neg_conclusion.push_back(token);
  }
  neg_conclusion.push_back(Token(Ops::RP));
}

void Solver::solve() {
  tree.parse(tokens);
  get_clauses(tree.head);
  conc_tree.parse(neg_conclusion);
  get_clauses(conc_tree.head);

  bool contradiction = find_contradiction();
  if (contradiction) {
    std::cout << "CONTRADICTION FOUND\n";
  } else {
    std::cout << "NO Contradiction\n";
  }
}
