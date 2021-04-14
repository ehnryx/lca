/* Longest Increasing Subsequence
 * USAGE
 *  auto lis = longest_inc_subseq(arr, inf, strict);
 * INPUT
 *  arr: array of which we want the LIS
 *  inf: infinity, should satisfy arr[i] < inf
 *  strict: whether the subsequence should increase strictly
 * OUTPUT
 *  lis[i] = index of the ith element in the LIS
 * TIME
 *  O(NlogN)
 *  N = |array|
 * STATUS
 *  tested: kattis/longincsubseq
 */
#pragma once

template <typename T>
vector<int> longest_inc_subseq(
    const vector<T>& arr,
    T inf,
    bool strict = true) {
  if (arr.empty()) return {};
  vector<pair<T, int>> inc(size(arr), pair(inf, 0));
  vector<int> previous(size(arr), -1);
  for (int i = 0; i < size(arr); i++) {
    auto it = lower_bound(begin(inc), end(inc), pair(arr[i], !strict * i));
    if (it != begin(inc)) previous[i] = prev(it)->second;
    *it = pair(arr[i], i);
  }
  auto it = lower_bound(begin(inc), end(inc), pair(inf, 0));
  vector<int> lis;
  for (int i = prev(it)->second; i != -1; i = previous[i]) {
    lis.push_back(i);
  }
  reverse(begin(lis), end(lis));
  return lis;
}

