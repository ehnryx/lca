/* Longest Increasing Subsequence
 * USAGE
 *  auto lis = longest_inc_subseq(arr, strict);
 * INPUT
 *  arr: array of which we want the LIS
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

#include <vector>

template <typename T>
std::vector<int> longest_inc_subseq(const std::vector<T>& arr, bool strict = true) {
  if (arr.empty()) return {};
  std::vector<std::pair<T, int>> inc;
  std::vector<int> previous(size(arr), -1);
  for (int i = 0; i < (int)size(arr); i++) {
    auto it = lower_bound(begin(inc), end(inc), std::pair(arr[i], !strict * i));
    if (it != begin(inc)) previous[i] = prev(it)->second;
    if (it == end(inc)) {
      inc.emplace_back(arr[i], i);
    } else {
      *it = std::pair(arr[i], i);
    }
  }
  std::vector<int> lis;
  for (int i = inc.back().second; i != -1; i = previous[i]) {
    lis.push_back(i);
  }
  reverse(begin(lis), end(lis));
  return lis;
}

