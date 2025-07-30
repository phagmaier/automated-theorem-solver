#pragma once
#include "dynamicbitset.h"

struct Clause {
  DynamicBitset positive_mask;
  DynamicBitset negative_mask;

  // The constructor now creates bitsets of the correct size
  Clause(size_t num_propositions)
      : positive_mask(num_propositions), negative_mask(num_propositions) {}

  // DynamicBitset &operator|=(const DynamicBitset &other);
  bool operator<(const Clause &other) const;
};

/*
// Your resolution check becomes:
DynamicBitset clashes =
    (A.positive_mask & B.negative_mask) | (A.negative_mask & B.positive_mask);

if (clashes.count() == 1) {
  // Valid resolution
}

// Your check for the empty clause becomes:
if (new_clause.positive_mask.is_zero() && new_clause.negative_mask.is_zero()) {
  // Contradiction found!
}
*/
