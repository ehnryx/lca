/* Range Minimum Query
 * USAGE
 *  range_minimum_query rmq(arr);
 *  int val = rmq.query(l, r); // inclusive-exclusive range [l, r)
 * CONSTRUCTOR
 *  arr: array on which to build the range minimum query
 * TIME
 *  O(NlogN) construction, O(1) query
 *  N = |array|
 * STATUS
 *  tested: kattis/tourists, cf/1454f
 */
#pragma once

template <typename T, class Compare = less<>>
struct range_minimum_query {
  vector<vector<T>> rmq;
  range_minimum_query() = default;
  range_minimum_query(const vector<T>& arr) {
    int n = (int)arr.size();
    int L = 32 - __builtin_clz(n);
    rmq.resize(L);
    rmq.front() = arr;
    build(n, L);
  }
  range_minimum_query(vector<T>&& arr) {
    int n = (int)arr.size();
    int L = 32 - __builtin_clz(n);
    rmq.resize(L);
    rmq.front() = move(arr);
    build(n, L);
  }
  T query(int l, int r) const {
    if (l >= r) throw invalid_argument("The range is empty, ie. l >= r");
    int j = 31 - __builtin_clz(r - l);
    return min(rmq[j][l], rmq[j][r - (1<<j)], Compare());
  }

private:
  void build(int n, int L) {
    for (int j = 1; j < L; j++) {
      rmq[j].resize(n - (1 << j) + 1);
      for (int i = 0; i + (1 << j) <= n; i++) {
        rmq[j][i] = min(rmq[j-1][i], rmq[j-1][i + (1<<(j-1))], Compare());
      }
    }
  }
};

