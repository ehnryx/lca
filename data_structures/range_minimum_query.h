/* Range Minimum Query
 * USAGE
 *  range_minimum_query rmq(arr);
 *  int val = rmq.query(l, r); // inclusive-exclusive range [l, r)
 * CONSTRUCTOR
 *  arr: array on which to build the range minimum query
 * TIME
 *  O(NlogN) construction, O(1) per query
 *  N = |array|
 * STATUS
 *  tested: kattis/tourists, cf/1454/f
 */
#pragma once

template <typename T>
struct range_minimum_query : vector<vector<T>> {
  range_minimum_query() = default;
  range_minimum_query(const vector<T>& arr) {
    build(arr);
  }
  void build(const vector<T>& arr) {
    int L = 32 - __builtin_clz((int)size(arr));
    this->resize(L, arr);
    for (int j = 1; j < L; j++) {
      for (int i = 0; i + (1 << j) <= (int)size(arr); i++) {
        this->data()[j][i] = min(this->at(j-1)[i], this->at(j-1)[i + (1<<(j-1))]);
      }
    }
  }
  T query(int l, int r) {
    assert(l < r);
    int j = 31 - __builtin_clz(r - l);
    return min(this->at(j)[l], this->at(j)[r - (1<<j)]);
  }
};

