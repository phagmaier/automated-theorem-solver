# 💻 A Simple Theorem Prover

## ⚠️ Under Construction

This 'Prover' is not yet complete. Do not use yet. You are more than welcome to watch me struggle along the way or even provide help.

## ✨ Project Description

This is a foundational theorem prover designed to analyze and validate logical arguments written in a simple, human-readable format. The tool takes a file containing a set of propositional declarations, a main logical formula, and a conclusion, and determines whether the conclusion logically follows from the premises. This project serves as a learning exercise in parsing, logical transformation, and automated proof techniques.

## ✅ Features

* **Propositional Logic:** Supports basic propositional variables.
* **Intuitive Syntax:** Uses a clear, file-based syntax with keywords and delimiters.
* **Standard Operators:** Implements a core set of logical operators including AND, OR, NOT, IMPLIES, and IFF.
* **Structured Input:** Enforces a clean separation between proposition declarations, premises, and the final conclusion.

## 🚀 Getting Started

To compile and run the prover, follow these steps:

1.  Place your input file (e.g., `input.txt`) in the project directory.
2.  Compile the C++ source files using a standard C++ compiler like g++:
    ```sh
    g++ -o prover main.cpp lexer.cpp <other_files>.cpp
    ```
3.  Run the executable, passing your input file as an argument:
    ```sh
    ./prover input.txt
    ```

## 📝 Input Syntax

The input file follows a specific, structured format:

1.  **Proposition Declarations:** Start with the keyword `prop`.
2.  **Premise Expression:** Encased within `$` delimiters. Can be multi-line.
3.  **Conclusion:** A single line following the premises, also encased in `$`.

**Example:**

```
prop P
prop Q
P −> Q & ~P

P
```
```

*Note: This example is missing the conclusion for demonstration. The final argument structure depends on the chosen proof method.*

## 📋 TODO

- [ ] Conver to to Clause/dynamic bitset type
- [ ] Make unordered set work with clause type
- [x] Create the recursive descent parser
- [x] Once in tree format simplify modify and cancel out
- [ ] Parse/Convert conclusion to CRF Form and negate it
- [ ] Make language more verbose create better syntax
- [ ] Allow for more difficult kinds of theorems 

* ```

## 📄 License

Distributed under the BSD 3-Clause. See `LICENSE` for more information.
