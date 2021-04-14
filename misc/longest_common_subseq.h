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

template <typename T>
vector<T> longest_common_subseq(
    const vector<T>& a,
    const vector<T>& b) {
  vector length(size(a) + 1, vector<int>(size(b) + 1));
  for (int i = 0; i <= size(a); i++) {
    for (int j = 0; j <= size(b); j++) {
      if (i > 0) length[i][j] = max(length[i][j], length[i-1][j]);
      if (j > 0) length[i][j] = max(length[i][j], length[i][j-1]);
      if (i > 0 && j > 0 && a[i-1] == b[j-1]) {
        length[i][j] = max(length[i][j], 1 + length[i-1][j-1]);
      }
    }
  }
  vector<T> lcs;
  for (int i = size(a), j = size(b); i > 0 && j > 0; ) {
    if (a[i-1] == b[j-1]) {
      lcs.push_back(a[i-1]);
      i--; j--;
    } else if(length[i-1][j] == length[i][j]) {
      i--;
    } else {
      j--;
    }
  }
  reverse(begin(lcs), end(lcs));
  return lcs;
}
