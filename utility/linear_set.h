/* Bitset
 * USAGE
 *  linear_set arr(n, init [default=false]);
 *  arr[i] for write access
 *  arr.at(i, j, k) for read-only access
 *  arr.insert(i, j, k), arr.erase(i, j, k)
 *  no bounds-checking
 * NOTES
 *  stored in row-major order in a flattened vector
 *  unable to resize
 * STATUS
 *  untested
 */
#pragma once

#include <bit>
#include <cstdint>
#include <limits>
#include <vector>

struct linear_set {
  using type = uint64_t;
  static constexpr size_t width = std::numeric_limits<type>::digits;
  size_t const n;
  std::vector<type> data;
  linear_set(size_t maxn, bool init = false)
      : n(maxn), data((n + width - 1) / width, init ? ~type(0) : type(0)) {}
  size_t size() const { return n; }

  struct accessor {
    type* const data_ptr;
    size_t const index;
    accessor(type* d, size_t i) : data_ptr(d), index(i) {}
    operator bool() const { return data_ptr[index / width] >> (index % width) & 1; }
    void operator=(bool value) {
      auto const data_i = index / width;
      auto const shift = index % width;
      data_ptr[data_i] ^= ((data_ptr[data_i] >> shift & 1) ^ value) << shift;
    }
  };
  bool operator[](size_t index) const { return data[index / width] >> (index % width) & 1; }
  accessor operator[](size_t index) { return accessor(data.data(), index); }
  bool at(size_t index) const { return operator[](index); }

  void insert(size_t index) { data[index / width] |= type(1) << (index % width); }
  void erase(size_t index) { data[index / width] &= ~(type(1) << (index % width)); }

  size_t lower_bound(size_t index) const {
    if (auto const value = data[index / width] >> (index % width); value) {
      return index + std::countr_zero(value);
    }
    for (size_t i = index / width + 1; i < data.size(); i++) {
      if (data[i]) {
        return i * width + std::countr_zero(data[i]);
      }
    }
    return size();
  }

  size_t count() const {
    size_t c = 0;
    for (type block : data) {
      c += std::popcount(block);
    }
    if (n % width) {
      c -= std::popcount(data.back() >> (n % width));
    }
    return c;
  }
  size_t count(int l, int r) const {
    r += 1;  // right-exclusive
    size_t const left_i = l / width;
    size_t const right_i = r / width;
    if (left_i == right_i) {
      return std::popcount(data[left_i] >> (l % width)) -
             std::popcount(data[right_i] >> (r % width));
    }
    size_t c = std::popcount(data[left_i] >> (l % width)) +
               std::popcount(data[right_i] << (width - r % width));
    for (size_t i = left_i + 1; i < right_i; i++) {
      c += std::popcount(data[i]);
    }
    return c;
  }
};
