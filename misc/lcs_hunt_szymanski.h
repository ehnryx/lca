/* Longest Common Subsequence (Hunt-Szymanski)
 * https://www.cs.bgu.ac.il/~dpaa111/wiki.files/HuntSzymanski.pdf
 * USAGE
 *  auto lcs = longest_common_subseq(a, b);
 * INPUT
 *  a, b: two arrays of which we want the LCS
 * OUTPUT
 *  lcs[i] = value of the ith element in the LCS
 * TIME
 *  O(RlogN + NlogN)
 *  N = |array|, R = #(matched pairs)
 * STATUS
 *  tested: dmoj/lcs
 */
#pragma once

template <template<class, class> class map_t = unordered_map, typename T>
vector<T> lcs_hunt_szymanski(const vector<T>& a, const vector<T>& b) {
  if (size(a) < size(b)) return lcs_hunt_szymanski(b, a);
  // build matchlists
  map_t<T, vector<int>> matchlist;
  for (int i = (int)size(b) - 1; i >= 0; i--) {
    matchlist[b[i]].push_back(i);
  }
  // initialize thresholds and links
  struct Item : pair<T, Item*> {
    Item(T v, Item* previous): pair<T, Item*>(v, previous) {}
  };
  vector<Item*> link(size(a));
  vector<int> thresh(size(a), (int)size(b));
  // find lcs
  for (int i = 0; i < (int)size(a); i++) {
    auto ub = end(thresh);
    for (int j : matchlist[a[i]]) {
      ub = lower_bound(begin(thresh), ub, j);
      int k = (int)(ub - begin(thresh));
      if (j < thresh[k]) {
        thresh[k] = j;
        link[k] = new Item(a[i], k > 0 ? link[k-1] : nullptr);
      }
    }
  }
  // recover lcs
  int k = (int)(lower_bound(begin(thresh), end(thresh), size(b)) - begin(thresh));
  if (k == 0) return {};
  vector<T> lcs(k);
  for (Item* cur = link[k-1]; cur != nullptr; cur = cur->second) {
    lcs[--k] = cur->first;
  }
  return lcs;
}

