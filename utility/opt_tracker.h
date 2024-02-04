/* Opt Tracker
 * USAGE
 *  keeps track of best O(1) values (min or max)
 * STATUS
 *  untested
 */
#pragma once

#include <functional>
#include <limits>

template <
    typename T, size_t N, T default_value = std::numeric_limits<T>::max(),
    typename Compare = std::less<>>
  requires(N >= 1)
struct opt_tracker {
  std::array<T, N> data;
  opt_tracker() { clear(); }
  void clear() { std::fill(data.begin(), data.end(), default_value); }
  T const& get(int i) const { return data[i]; }
  T const& operator[](int i) const { return get(i); }
  T& get(int i) { return data[i]; }
  T& operator[](int i) { return get(i); }
  void push(T x) {
    for (int i = 0; i < N; i++) {
      if (Compare()(x, data[i])) {
        std::swap(x, data[i]);
      }
    }
  }
  void push_unique(T x) {
    for (int i = 0; i < N; i++) {
      if (x == data[i]) return;
      if (Compare()(x, data[i])) {
        std::swap(x, data[i]);
      }
    }
  }
};

template <
    typename T, size_t N, T default_value = std::numeric_limits<T>::max(),
    typename Compare = std::less<>>
  requires(N >= 1)
struct opt_tracker_multi {
  std::array<T, N> data;
  std::array<size_t, N> multiplicity;

  opt_tracker_multi() { clear(); }
  void clear() {
    std::fill(data.begin(), data.end(), default_value);
    std::fill(multiplicity.begin(), multiplicity.end(), 0);
  }
  std::pair<T const&, size_t> get(int i) const {
    return std::pair(std::cref(data[i]), multiplicity[i]);
  }
  std::pair<T const&, size_t> operator[](int i) const { return get(i); }
  T const& value(int i) const { return data[i]; }
  size_t count(int i) const { return multiplicity[i]; }
  void push(std::pair<T, size_t> p) {
    return push(p.first, p.second);
  }
  void push(T x, size_t mult = 1) {
    for (int i = 0; i < N; i++) {
      if (x == data[i]) {
        multiplicity[i] += mult;
        return;
      }
      if (Compare()(x, data[i])) {
        std::swap(x, data[i]);
        std::swap(mult, multiplicity[i]);
      }
    }
  }
};
