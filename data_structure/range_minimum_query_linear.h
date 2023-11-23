/* Range Minimum Query (Linear Construction)
 * USAGE
 *  range_minimum_query_linear rmq(arr);
 *  int val = rmq.query(l, r); // inclusive range [l, r]
 * CONSTRUCTOR
 *  arr: array on which to build the range minimum query
 * TIME
 *  O(N) construction, O(1) query
 *  N = |array|
 * STATUS
 *  untested
 */
#pragma once

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, typename Compare = std::less<T>>
struct range_minimum_query_linear {
  static constexpr int S = 32;
  std::vector<std::vector<T>> block_rmq;
  std::vector<T> values;
  std::vector<uint32_t> block_mask;
  range_minimum_query_linear(const std::vector<T>& arr) : values(arr), block_mask(arr.size()) {
    const int n = (int)arr.size();
    const int n_blocks = (n + S - 1) / S;
    const int L = 32 - __builtin_clz(n_blocks);
    block_rmq.resize(L, std::vector<T>(n_blocks));
    block_mask[0] = 1;
    block_rmq[0][0] = arr[0];
    for (int i = 1; i < n; i++) {
      const int bi = i / S;
      if (i % S == 0 or Compare()(arr[i], block_rmq[0][bi])) {
        block_rmq[0][bi] = arr[i];
      }
      block_mask[i] = block_mask[i - 1] << 1;
      while (block_mask[i] != 0) {
        const int j = __builtin_ctz(block_mask[i]);
        if (Compare()(arr[i - j], arr[i])) {
          break;
        } else {
          block_mask[i] ^= 1u << j;
        }
      }
      block_mask[i] |= 1;
    }
    for (int j = 1; j < L; j++) {
      for (int i = 0; i + (1 << j) <= n_blocks; i++) {
        block_rmq[j][i] =
            std::min(block_rmq[j - 1][i], block_rmq[j - 1][i + (1 << (j - 1))], Compare());
      }
    }
  }
  T query(int l, int r) const {
    if (l > r) throw std::invalid_argument("query is empty, ie. l > r");
    const int lb = l / S;
    const int rb = r / S;
    switch (rb - lb) {
      case 0:
        return query_small(l, r);
      case 1:
        return std::min(query_small(l, rb * S - 1), query_small(rb * S, r), Compare());
      default:
        return std::min(
            {query_big(lb + 1, rb - 1), query_small(l, (lb + 1) * S - 1),
             query_small(rb * S, r)},
            Compare());
    }
  }
  T query_small(int l, int r) const {
    const uint32_t mask = block_mask[r] & (~0u >> (S - (r + 1 - l)));
    return values[r - (S - __builtin_clz(mask) - 1)];
  }
  T query_big(int l, int r) const {
    const int j = 31 - __builtin_clz(r + 1 - l);
    return std::min(block_rmq[j][l], block_rmq[j][r + 1 - (1 << j)], Compare());
  }
};
