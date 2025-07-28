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
int main(int argc, char *argv[]) {
  try {
    /*
    if (argc < 2) {
      err_exit(NO_ARGS);
    }

    Lexer my_lexer(argv[1]);

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  */
    const char *fileName = "../data/solver.txt";
    // Lexer lexer = Lexer(fileName);
    Solver solver = Solver(fileName);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
