# 💻 Theorem Prover

## ⚠️ Under Construction/Work in Progress

This project currently functions as a basic theorem prover for **propositional logic**, validating arguments without quantifiers. While ambitiously aiming for features like quantifier support and advanced proof automation (acknowledging established tools like Coq), its primary purpose is a **learning exercise** in logical system development.

The current implementation uses **proof by contradiction** to determine theorem validity. It shows the transformation of the input theorem and conclusion into **Conjunctive Normal Form (CNF)** but does not yet provide explicit step-by-step proof derivations.

---

## ✨ Project Description

This automated theorem prover is designed to analyze and validate logical arguments written in a simple, human-readable format. It processes an input file containing propositional declarations, a set of logical premises, and a conclusion. The tool then determines whether the conclusion logically follows from the premises. This project serves as a practical learning exercise in parsing, logical transformation, and automated proof techniques.

---

## ✅ Features

* **Propositional Logic:** Supports fundamental propositional variables.
* **Intuitive Syntax:** Utilizes a clear, file-based syntax with dedicated keywords and delimiters for structured input.
* **Standard Operators:** Implements a core set of logical operators including **AND**, **OR**, **NOT**, **IMPLIES** (`->`, `imp`, `IMP`), and **IFF** (`<->`, `iff`, `IFF`).
* **Structured Input:** Enforces a clean separation between proposition declarations, premises, and the final conclusion for clarity.

---

## 🚀 Getting Started

To compile and run the prover, follow these steps:

1.  Write your basic theorem in [`solver.txt`](data/solver.txt)
2.  I have provided a CMAKE file that you can use to build the program. Or You can manually compile. All files needed for building are in the CMAKE file
3. To compile with cmake `cmake -B build` and then `cmake --build build`
4. I also wrote two short bash scripts to compile & run. `./clean_build.sh` to clean build and run and `./runme.sh` to build and run
5. If you manually compiled using either cmake the executable should be in the `/build` directory and the executable should be called `runme` to run just run the executable `./runme`
 
---

## 📝 Input Syntax

* The language is **case-sensitive**.
* All operations must be separated by **spaces**, with the exception of single-character operators (`~`, `(`, `)`).

**Reserved words/Operations:**
* `AND`, `and`
* `OR`, `or`
* `NOT`, `not`
* `NEG`, `neg`, `~`
* `Prop`, `prop`, `PROP`
* `->`, `imp`, `IMP`
* `<->`, `iff`, `IFF`
* `(`, `)`

**File Structure:**

1.  **Proposition Declarations:**
    * Must begin with the keyword `prop`.
    * Each proposition declaration must be on a single line.
    * Proposition names **cannot be reserved words** and should only contain alphanumerics.
    * **Example:** `prop P`

2.  **Premise Expression:**
    * Encased within `$` delimiters.
    * Can span multiple lines.
    * **Example:** `$P -> Q & ~P$`

3.  **Conclusion:**
    * Any content after the premises (`$`) is considered the conclusion.
    * Can be multi-line.
    * **Example:**
        ```
        Q
        ```

**Complete Example:**

```
prop P
prop Q
$P -\> Q & \~P$

Q
```



---

## 📂 Project Structure (Major Files)

| File Name                                   | Description                                                                     |
| :------------------------------------------ | :------------------------------------------------------------------------------ |
| [`main.cpp`](src/main.cpp)              | The primary entry point; orchestrates the theorem prover's execution flow.      |
| [`lexer.cpp`](src/lexer.cpp)            | Handles **lexical analysis**, breaking raw input text into meaningful tokens.   |
| [`tree.cpp`](src/tree.cpp)              | **Parses** the tokens to construct an Abstract Syntax Tree (AST) in CNF form.  |
| [`dynamicbitset.cpp`](src/dynamicbitset.cpp) | Provides an efficient, dynamic bitset representation for propositions.          |
| [`clause.cpp`](src/clause.cpp)          | Defines the `Clause` structure, representing collections of positive and negative literals (propositions) using a dynamic bitset. |
| [`solver.cpp`](src/solver.cpp)          | Contains the core **resolution-based theorem proving logic**.                   |
| [`solver.txt`](data/solver.txt)                           | File where theorems **MUST** be written to. (Will be changed soon.)          |
| `LICENSE`                                   | Contains the licensing information for the project.                             |
| `README.md`                                 | This documentation file.                                                        |

---

## 📋 TODO

* [ ] Add support for **quantifiers** (First-Order Logic).
* [ ] Improve language verbosity and syntax for enhanced readability and expressiveness.
* [ ] Allow for more complex theorem structures and advanced proof methods.
* [ ] Implement explicit **proof step generation** to show derivation details.
* [ ] Enhance file input: allow the theorem file to be located anywhere, not just in `data/solver.txt`.

---

## 📄 License

Distributed under the BSD 3-Clause License. See `LICENSE` for more information.
