#pragma once
#include "dynamicbitset.h"
#include <functional>

struct Clause {
  DynamicBitset positive_mask;
  DynamicBitset negative_mask;

  Clause(size_t num_propositions)
      : positive_mask(num_propositions), negative_mask(num_propositions) {}

  void set_positive(int pos) { positive_mask.set(pos); }
  void set_negative(int pos) { negative_mask.set(pos); }
  void unset_positive(int pos) { positive_mask.reset(pos); }
  void unset_negative(int pos) { negative_mask.reset(pos); }

  bool operator==(const Clause &other) const;
  bool operator<(const Clause &other) const;
};

namespace std {
template <> struct hash<Clause> {
  size_t operator()(const Clause &c) const noexcept {
    size_t h1 = std::hash<DynamicBitset>{}(c.positive_mask);
    size_t h2 = std::hash<DynamicBitset>{}(c.negative_mask);
    return h1 ^ (h2 << 1);
  }
};
} // namespace std
