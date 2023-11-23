/* Longest Common Subsequence
 * USAGE
 *  auto lcs = longest_common_subseq(a, b);
 * INPUT
 *  a, b: two arrays of which we want the LCS
 * OUTPUT
 *  lcs[i] = value of the ith element in the LCS
 * TIME
 *  O(N^2)
 *  N = |array|
 * STATUS
 *  dmoj/lcs
 */
#pragma once

#include <vector>

template <typename T>
std::vector<T> longest_common_subseq(const std::vector<T>& a, const std::vector<T>& b) {
  std::vector length(size(a) + 1, std::vector<int>(size(b) + 1));
  for (int i = 0; i <= (int)size(a); i++) {
    for (int j = 0; j <= (int)size(b); j++) {
      if (i > 0) length[i][j] = max(length[i][j], length[i - 1][j]);
      if (j > 0) length[i][j] = max(length[i][j], length[i][j - 1]);
      if (i > 0 && j > 0 && a[i - 1] == b[j - 1]) {
        length[i][j] = max(length[i][j], 1 + length[i - 1][j - 1]);
      }
    }
  }
  std::vector<T> lcs;
  for (int i = (int)size(a), j = (int)size(b); i > 0 && j > 0;) {
    if (a[i - 1] == b[j - 1]) {
      lcs.push_back(a[i - 1]);
      i--;
      j--;
    } else if (length[i - 1][j] == length[i][j]) {
      i--;
    } else {
      j--;
    }
  }
  reverse(begin(lcs), end(lcs));
  return lcs;
}
