/* Weighted Longest Increasing Subsequence
 * USAGE
 *  auto lis = lis_weighted(arr, inf, strict);
 * INPUT
 *  arr: array of pair(value, weight), of which we want the weighted LIS
 *  inf: infinity, should satisfy arr[i] < inf
 *  strict: whether the subsequence should increase strictly
 * OUTPUT
 *  lis[i] = index of the ith element in the LIS
 * TIME
 *  O(NlogN)
 *  N = |array|
 * STATUS
 *  tested: cf/101889f
 */
#pragma once

template <typename T, typename U>
vector<int> lis_weighted(const vector<pair<T, U>>& arr, bool strict = true) {
  if (arr.empty()) return {};
  map<pair<T, int>, U> inc;
  vector<int> previous(size(arr), -1);
  for (int i = 0; i < size(arr); i++) {
    pair<T, int> key(arr[i].first, strict ? -i : i);
    auto it = inc.lower_bound(key);
    for (U cur = arr[i].second; cur > 0 && it != end(inc); ) {
      if (it->second <= cur) {
        cur -= it->second;
        it = inc.erase(it);
      } else {
        it->second -= cur;
        cur = 0;
      }
    }
    it = inc.insert(pair(key, arr[i].second)).first;
    if (it != begin(inc)) {
      previous[i] = abs(prev(it)->first.second);
    }
  }
  vector<int> lis;
  for (int i = abs(inc.rbegin()->first.second); i != -1; i = previous[i]) {
    lis.push_back(i);
  }
  reverse(begin(lis), end(lis));
  return lis;
}

