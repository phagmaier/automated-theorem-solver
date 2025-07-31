#pragma once
#include <algorithm>
#include <bit>
#include <cmath>
#include <cstddef>
#include <functional>
#include <numeric>
#include <stdexcept>
#include <vector>

struct DynamicBitset {
  DynamicBitset();
  DynamicBitset(size_t num_bits);

  void set(size_t pos);
  void reset(size_t pos);
  bool test(size_t pos) const;
  void clear();

  size_t count() const;
  bool is_zero() const;
  size_t size() const;

  DynamicBitset &operator&=(const DynamicBitset &other);
  DynamicBitset &operator|=(const DynamicBitset &other);
  bool operator==(const DynamicBitset &other) const;
  bool operator!=(const DynamicBitset &other) const {
    return !(*this == other);
  }
  bool operator<(const DynamicBitset &other) const;

  static const int BITS_PER_BLOCK = 64;
  std::vector<unsigned long long> m_blocks;
  size_t m_num_bits;
};

DynamicBitset operator&(const DynamicBitset &lhs, const DynamicBitset &rhs);
DynamicBitset operator|(const DynamicBitset &lhs, const DynamicBitset &rhs);
DynamicBitset operator~(const DynamicBitset &bs);

namespace std {
template <> struct hash<DynamicBitset> {
  std::size_t operator()(const DynamicBitset &bs) const noexcept {
    std::size_t seed = 0;
    for (unsigned long long block : bs.m_blocks) {
      seed ^= std::hash<unsigned long long>{}(block) + 0x9e3779b9 +
              (seed << 6) + (seed >> 2);
    }
    seed ^= std::hash<size_t>{}(bs.m_num_bits) + 0x9e3779b9 + (seed << 6) +
            (seed >> 2);
    return seed;
  }
};
} // namespace std
