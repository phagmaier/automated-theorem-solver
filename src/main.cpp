#include "display_tree.h"
#include "solver.h"
/*RULES FOR INPUT
 * prop declares a prop
 * One decleration per line
 * can have blank lines
 * can have arbitrary amount of whitespace between symbols
 * (add comments later)
 * Must be a space between everything (except for (, ), ~)
 * Statment must be enclosed by $$
 * We can only evaluate one expression at a time (for now)
 * Only take propositions and operations (for now)
 * */

/*Precedence	Operator	Name
    Highest	( )	Parentheses
    ∀,∃	Quantifiers
    ¬	Negation
    ∧	Conjunction
    ∨	Disjunction
    Lowest	→,↔	Implication, Biconditional
*/

// In the future we want to write a bash script so
// user can write their solver data anyehre and then they'd run a command that
// we would write that would esssentially write their file to wherever
// this program and we'd run it
// for now to make things easier during building everything will be in the data
// folder in a file called file.txt
int main() {
  try {

    const char *fileName = "../data/solver.txt";
    // Lexer lexer = Lexer(fileName);
    // init solver
    std::cout << "STARTING SOLVER\n";
    Solver solver = Solver(fileName);
    std::cout << "SOLVER Initialized\n";
    std::cout << "SOLVING \n";
    solver.solve();
    std::cout << "Solved\n";
    generate_graphviz_file(solver, "ast.dot");
    std::cout << "MADE GRAPH\n";
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
