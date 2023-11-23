/* Longest Common Subsequence (Finite Alphabet)
 * https://users.monash.edu/~lloyd/tildeStrings/Alignment/86.IPL.html
 * USAGE
 *  int lcs = lcs_bitstring<N>(a, b);
 * INPUT
 *  a, b: arrays of which we want the LCS
 *    a should have a fixed alphabet
 *  N: length of bitstrings, should satisfy N > |a|
 * OUTPUT
 *  the length of the longest common subsequence of a and b
 * TIME
 *  O(NM/64 + NA))
 *  N = |a|, M = |b|, A = size of alphabet of a
 * STATUS
 *  tested: spoj/LCS0, dmoj/lcs
 */
#pragma once

#include <map>
#include <vector>

template <typename word_t = unsigned long long, typename T>
int lcs_bitstring(const std::vector<T>& a, const std::vector<T>& b) {
  if (size(a) > size(b)) return lcs_bitstring(b, a);
  int m = 0;
  std::map<T, int> alphabet;
  for (const T& c : a) {
    if (!alphabet.count(c)) {
      alphabet[c] = m++;
    }
  }
  // precompute the bitstrings
  constexpr int word = 8 * sizeof(word_t);
  const int n = ((int)size(a) + word - 1) / word;
  std::vector bits(m, vector<word_t>(n));
  for (auto [c, j] : alphabet) {
    for (int i = 0; i < (int)size(a); i++) {
      if (a[i] == c) {
        bits[j][i / word] |= (word_t)1 << (i % word);
      }
    }
  }
  // update lcs
  std::vector<word_t> row(n);
  for (int i = 0; i < (int)size(b); i++) {
    if (!alphabet.count(b[i])) continue;
    const auto& bs = bits[alphabet[b[i]]];
    for (int j = 0, carry = 1, borrow = 0; j < n; j++) {
      word_t x = row[j] | bs[j];
      bool new_carry = row[j] >> (word - 1) & 1;
      row[j] = x - (row[j] << 1 | carry) - borrow;
      borrow = (borrow && !(row[j] + 1)) || row[j] + borrow > x;
      row[j] = (row[j] ^ x) & x;
      carry = new_carry;
    }
  }
  return accumulate(
      begin(row), end(row), 0, [](int c, word_t v) { return c + __builtin_popcountll(v); });
}
