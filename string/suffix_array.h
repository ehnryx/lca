/* Suffix Array (Induced Sort)
 * source: Two Efficient Algorithms for Linear Time Suffix Array Construction
 *         (Ge Nong, Sen Zhang, Daricks Wai Hong Chan)
 * USAGE
 *  suffix_array sa(string_s)
 * INPUT
 *  string or basic_string<int>
 *  remap values to [1,n] if using basic_string<int>. DO NOT use 0
 * OUTPUT
 *  sa = suffix array. sa[0] = n (the sentinel) always
 *  inv = inverse of sa. result is 1-indexed because inv[n] = 0
 *  height = height of the lcp histogram. height[i] = lcp(sa[i], sa[i-1])
 *  operator[] to access sa
 * TIME
 *  O(N)
 *  N = |string|
 * STATUS
 *  tested: cf/102803c, spoj/LCS,LCS2, nadc21/b,h
 */
#pragma once

template <typename T>
struct suffix_array {
  vector<int> sa, inv, height;
  int operator [] (int i) const { return sa[i]; }

  suffix_array(const basic_string<T>& s) {
    int n = (int)size(s) + 1;
    vector<int> t(n);
    copy(begin(s), end(s), begin(t));
    sa = build(t, *max_element(begin(t), end(t)) + 1);
    // generate inverse of sa
    inv.resize(n);
    for (int i = 0; i < n; i++) {
      inv[sa[i]] = i;
    }
    // generate heights of the lcp histogram
    height.resize(n);
    for (int i = 0, h = 0; i < n; i++) {
      if (inv[i] == 0) continue;
      int j = sa[inv[i] - 1];
      while (i + h < n && j + h < n && t[i + h] == t[j + h]) {
        h++;
      }
      height[inv[i]] = h;
      if (h > 0) {
        h--;
      }
    }
  }

  vector<int> build(const vector<int>& s, int A) {
    int n = (int)size(s);
    // get type and lms indices
    vector<bool> ltype(n);
    vector<int> lms;
    for (int i = n - 2; i >= 0; i--) {
      ltype[i] = (s[i] > s[i + 1] || (s[i] == s[i + 1] && ltype[i + 1]));
      if (ltype[i] && !ltype[i + 1]) {
        lms.push_back(i + 1);
      }
    }
    reverse(begin(lms), end(lms));
    // induced_sort
    vector<int> suf = induced_sort(s, ltype, lms, A);
    // get lms order
    vector<int> lms_order(size(lms));
    for (int i = 0, j = 0; i < n; i++) {
      if (suf[i] > 0 && ltype[suf[i] - 1] && !ltype[suf[i]]) {
        lms_order[j++] = suf[i];
      }
    }
    // name lms substrings
    int name = 0;
    suf[n - 1] = 0;
    for (int i = 1; i < (int)size(lms); i++) {
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
    if (name + 1 < (int)size(lms)) {
      vector<int> s_lms(size(lms));
      for (int i = 0; i < (int)size(lms); i++) {
        s_lms[i] = suf[lms[i]];
      }
      vector<int> suf_lms = build(s_lms, name + 1);
      for (int i = 0; i < (int)size(lms); i++) {
        lms_order[i] = lms[suf_lms[i]];
      }
    }
    // induced_sort
    return induced_sort(s, ltype, lms_order, A);
  }

  vector<int> induced_sort(
      const vector<int>& s, const vector<bool>& ltype,
      const vector<int>& idx, int A) {
    vector<int> suf(size(s), -1);
    vector<int> left(A), right(A), r2(A);
    for (int c : s) {
      right[c] += 1;
    }
    partial_sum(begin(right), end(right), begin(right));
    copy(begin(right), end(right) - 1, begin(left) + 1);
    copy(begin(right), end(right), begin(r2));
    for (int i = (int)size(idx) - 1; i >= 0; i--) {
      suf[--right[s[idx[i]]]] = idx[i];
    }
    for (int i = 0; i < (int)size(s); i++) {
      if (suf[i] > 0 && ltype[suf[i] - 1]) {
        suf[left[s[suf[i] - 1]]++] = suf[i] - 1;
      }
    }
    for (int i = (int)size(s) - 1; i >= 0; i--) {
      if (suf[i] > 0 && !ltype[suf[i] - 1]) {
        suf[--r2[s[suf[i] - 1]]] = suf[i] - 1;
      }
    }
    return suf;
  }
};

