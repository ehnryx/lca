/* Suffix Array (Induced Sort)
 * source: Two Efficient Algorithms for Linear Time Suffix Array Construction
 *         (Ge Nong, Sen Zhang, Daricks Wai Hong Chan)
 * USAGE
 *  suffix_array sa(string_s)
 * INPUT
 *  string or basic_string<int>
 *  remap values to [1,n] if using basic_string<int>. DO NOT use 0
 * OUTPUT
 *  sa = suffix array. sa[0] = n (the sentinel) always (inverse of rank)
 *  rank = rank of suffix in sorted order. result is 1-indexed because rank[n] = 0
 *  height = height of the lcp histogram. height[i] = lcp(sa[i], sa[i-1])
 *  operator[] to access sa
 * TIME
 *  O(N)
 *  N = |string|
 * STATUS
 *  tested: cf/102803c, spoj/LCS,LCS2, nadc21/b,h
 */
#pragma once

#include <numeric>
#include <string>
#include <vector>

template <typename T>
struct suffix_array {
  std::vector<int> sa, rank, height;
  int operator [] (int i) const { return sa[i]; }
  int size() const { return (int)sa.size(); }

  suffix_array(const std::basic_string<T>& s) {
    int n = (int)s.size() + 1;
    std::vector<int> t(n);
    copy(begin(s), end(s), begin(t));
    sa = build(t, *max_element(begin(t), end(t)) + 1);
    // generate rank of suffix in sorted order
    rank.resize(n);
    for (int i = 0; i < n; i++) {
      rank[sa[i]] = i;
    }
    // generate heights of the lcp histogram
    height.resize(n);
    for (int i = 0, h = 0; i < n; i++) {
      if (rank[i] == 0) continue;
      int j = sa[rank[i] - 1];
      while (i + h < n && j + h < n && t[i + h] == t[j + h]) {
        h++;
      }
      height[rank[i]] = h;
      if (h > 0) {
        h--;
      }
    }
  }

  std::vector<int> build(const std::vector<int>& s, int A) {
    int n = (int)s.size();
    // get type and lms indices
    std::vector<bool> ltype(n);
    std::vector<int> lms;
    for (int i = n - 2; i >= 0; i--) {
      ltype[i] = (s[i] > s[i + 1] || (s[i] == s[i + 1] && ltype[i + 1]));
      if (ltype[i] && !ltype[i + 1]) {
        lms.push_back(i + 1);
      }
    }
    reverse(begin(lms), end(lms));
    // induced_sort
    std::vector<int> suf = induced_sort(s, ltype, lms, A);
    // get lms order
    std::vector<int> lms_order(lms.size());
    for (int i = 0, j = 0; i < n; i++) {
      if (suf[i] > 0 && ltype[suf[i] - 1] && !ltype[suf[i]]) {
        lms_order[j++] = suf[i];
      }
    }
    // name lms substrings
    int name = 0;
    suf[n - 1] = 0;
    for (int i = 1; i < (int)lms.size(); i++) {
      bool diff = false;
      for (int j = 0; ; j++) {
        if (s[lms_order[i] + j] != s[lms_order[i - 1] + j]
            || ltype[lms_order[i] + j] != ltype[lms_order[i - 1] + j]) {
          diff = true;
          break;
        } else if (j > 0 && ((ltype[lms_order[i] + j - 1] && !ltype[lms_order[i] + j])
              || (ltype[lms_order[i - 1] + j - 1] && !ltype[lms_order[i - 1] + j]))) {
          break;
        }
      }
      name += diff;
      suf[lms_order[i]] = name;
    }
    // recurse if not unique
    if (name + 1 < (int)lms.size()) {
      std::vector<int> s_lms(lms.size());
      for (int i = 0; i < (int)lms.size(); i++) {
        s_lms[i] = suf[lms[i]];
      }
      std::vector<int> suf_lms = build(s_lms, name + 1);
      for (int i = 0; i < (int)lms.size(); i++) {
        lms_order[i] = lms[suf_lms[i]];
      }
    }
    // induced_sort
    return induced_sort(s, ltype, lms_order, A);
  }

  std::vector<int> induced_sort(
      const std::vector<int>& s, const std::vector<bool>& ltype,
      const std::vector<int>& idx, int A) {
    std::vector<int> suf(s.size(), -1);
    std::vector<int> left(A), right(A), r2(A);
    for (int c : s) {
      right[c] += 1;
    }
    std::partial_sum(begin(right), end(right), begin(right));
    std::copy(begin(right), end(right) - 1, begin(left) + 1);
    std::copy(begin(right), end(right), begin(r2));
    for (int i = (int)idx.size() - 1; i >= 0; i--) {
      suf[--right[s[idx[i]]]] = idx[i];
    }
    for (int i = 0; i < (int)s.size(); i++) {
      if (suf[i] > 0 && ltype[suf[i] - 1]) {
        suf[left[s[suf[i] - 1]]++] = suf[i] - 1;
      }
    }
    for (int i = (int)s.size() - 1; i >= 0; i--) {
      if (suf[i] > 0 && !ltype[suf[i] - 1]) {
        suf[--r2[s[suf[i] - 1]]] = suf[i] - 1;
      }
    }
    return suf;
  }
};

