#include "dynamicbitset.h"

DynamicBitset::DynamicBitset() : m_num_bits(0) {}

DynamicBitset::DynamicBitset(size_t num_bits) : m_num_bits(num_bits) {
  size_t num_blocks = (num_bits + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK;
  m_blocks.resize(num_blocks, 0);
}

void DynamicBitset::set(size_t pos) {
  if (pos >= m_num_bits)
    throw std::out_of_range("Bitset position out of range.");
  size_t block_index = pos / BITS_PER_BLOCK;
  size_t bit_index = pos % BITS_PER_BLOCK;
  m_blocks[block_index] |= (1ULL << bit_index);
}

void DynamicBitset::reset(size_t pos) {
  if (pos >= m_num_bits)
    throw std::out_of_range("Bitset position out of range.");
  size_t block_index = pos / BITS_PER_BLOCK;
  size_t bit_index = pos % BITS_PER_BLOCK;
  m_blocks[block_index] &= ~(1ULL << bit_index);
}

bool DynamicBitset::test(size_t pos) const {
  if (pos >= m_num_bits)
    return false;
  size_t block_index = pos / BITS_PER_BLOCK;
  size_t bit_index = pos % BITS_PER_BLOCK;
  return (m_blocks[block_index] & (1ULL << bit_index)) != 0;
}

void DynamicBitset::clear() { std::fill(m_blocks.begin(), m_blocks.end(), 0); }

size_t DynamicBitset::count() const {
  size_t total_count = 0;
  for (unsigned long long block : m_blocks) {
    total_count += std::popcount(block);
  }
  return total_count;
}

bool DynamicBitset::is_zero() const {
  for (unsigned long long block : m_blocks) {
    if (block != 0) {
      return false;
    }
  }
  return true;
}

size_t DynamicBitset::size() const { return m_num_bits; }

bool DynamicBitset::operator==(const DynamicBitset &other) const {
  if (m_num_bits != other.m_num_bits) {
    return false;
  }
  return std::equal(m_blocks.begin(), m_blocks.end(), other.m_blocks.begin());
}

DynamicBitset &DynamicBitset::operator&=(const DynamicBitset &other) {
  if (m_num_bits != other.m_num_bits) {
    throw std::invalid_argument(
        "Bitset sizes must match for bitwise AND assignment.");
  }
  for (size_t i = 0; i < m_blocks.size(); ++i) {
    m_blocks[i] &= other.m_blocks[i];
  }
  return *this;
}

DynamicBitset &DynamicBitset::operator|=(const DynamicBitset &other) {
  if (m_num_bits != other.m_num_bits) {
    throw std::invalid_argument(
        "Bitset sizes must match for bitwise OR assignment.");
  }
  for (size_t i = 0; i < m_blocks.size(); ++i) {
    m_blocks[i] |= other.m_blocks[i];
  }
  return *this;
}

bool DynamicBitset::operator<(const DynamicBitset &other) const {
  size_t max_blocks_count = std::max(m_blocks.size(), other.m_blocks.size());

  for (size_t i = 0; i < max_blocks_count; ++i) {
    size_t this_block_idx = (m_blocks.size() - 1) - i;
    size_t other_block_idx = (other.m_blocks.size() - 1) - i;

    unsigned long long this_block_val = 0ULL;
    if (this_block_idx < m_blocks.size()) {
      this_block_val = m_blocks[this_block_idx];
    }

    unsigned long long other_block_val = 0ULL;
    if (other_block_idx < other.m_blocks.size()) {
      other_block_val = other.m_blocks[other_block_idx];
    }

    if (this_block_val < other_block_val) {
      return true;
    }
    if (this_block_val > other_block_val) {
      return false;
    }
  }
  return false;
}

DynamicBitset operator&(const DynamicBitset &lhs, const DynamicBitset &rhs) {
  DynamicBitset result = lhs;
  result &= rhs;
  return result;
}

DynamicBitset operator|(const DynamicBitset &lhs, const DynamicBitset &rhs) {
  DynamicBitset result = lhs;
  result |= rhs;
  return result;
}

DynamicBitset operator~(const DynamicBitset &bs) {
  DynamicBitset result = bs; // Make a copy

  // Invert all the bits in the full blocks
  for (size_t i = 0; i < result.m_blocks.size() - 1; ++i) {
    result.m_blocks[i] = ~result.m_blocks[i];
  }

  // Handle the last block carefully
  if (!result.m_blocks.empty()) {
    size_t last_block_idx = result.m_blocks.size() - 1;
    result.m_blocks[last_block_idx] = ~result.m_blocks[last_block_idx];

    // Create a mask to clear out the unused bits
    size_t bits_in_last_block = bs.size() % DynamicBitset::BITS_PER_BLOCK;
    if (bits_in_last_block > 0) {
      // This mask will have 1s for the bits that are actually in use
      unsigned long long mask = (1ULL << bits_in_last_block) - 1;
      result.m_blocks[last_block_idx] &= mask;
    }
  }

  return result;
}
