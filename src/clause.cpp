#include "clause.h"

bool Clause::operator==(const Clause &other) const {
  return positive_mask == other.positive_mask &&
         negative_mask == other.negative_mask;
}

bool Clause::operator<(const Clause &other) const {
  if (positive_mask < other.positive_mask)
    return true;
  if (other.positive_mask < positive_mask)
    return false;
  return negative_mask < other.negative_mask;
}
