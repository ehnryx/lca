/* Capped Int
 * USAGE
 *  capped_int<cap> v(num);
 *  supports + and *
 * STATUS
 *  somewhat tested: kattis/kthsubtree
 */
#pragma once

#include <algorithm>
#include <limits>

template <unsigned long long cap_ll>
struct capped_int {
  using capped_t = std::conditional_t<
      (cap_ll < std::numeric_limits<int>::max()), unsigned int, unsigned long long>;
  static constexpr capped_t cap = (capped_t)cap_ll;
  capped_t v;
  capped_int() : v(0) {}
  capped_int(unsigned int _v) : v(min((capped_t)_v, cap)) {}
  capped_int(unsigned long long _v) : v((capped_t)std::min(_v, (unsigned long long)cap)) {}
  capped_int(int _v) {
    if (_v < 0)
      throw std::invalid_argument("cannot create a capped_int from a negative number");
    v = min((capped_t)_v, cap);
  }
  capped_int(long long _v) {
    if (_v < 0)
      throw std::invalid_argument("cannot create a capped_int from a negative number");
    v = (capped_t)std::min((unsigned long long)_v, (unsigned long long)cap);
  }

  capped_int operator+(const capped_int& o) const { return capped_int(*this) += o; }
  capped_int operator*(const capped_int& o) const { return capped_int(*this) *= o; }
  capped_int& operator+=(const capped_int& o) {
    v = min(cap, v + o.v);
    return *this;
  }
  capped_int& operator*=(const capped_int& o) {
    if (v == 0 || o.v == 0) {
      v = 0;
    } else if (v > cap / o.v + 1) {
      v = cap;
    } else {
      v = min(cap, v * o.v);
    }
    return *this;
  }

  bool operator<(const capped_int& o) const { return v < o.v; }
  bool operator<(capped_t o) const { return v < o; }
  bool operator<(int o) const { return o > 0 && v < (unsigned int)o; }
  bool operator<(long long o) const { return o > 0 && v < (unsigned long long)o; }
  bool operator==(const capped_int& o) const { return v == o.v; }
  bool operator==(capped_t o) const { return v == o; }
  bool operator==(int o) const { return o >= 0 && v == (unsigned int)o; }
  bool operator==(long long o) const { return o >= 0 && v == (unsigned long long)o; }
  template <typename other_t>
  bool operator<=(const other_t& o) const {
    return operator==(o) || operator<(o);
  }
  template <typename other_t>
  bool operator>(const other_t& o) const {
    return !operator<=(o);
  }
  template <typename other_t>
  bool operator>=(const other_t& o) const {
    return !operator<(o);
  }
  friend std::ostream& operator<<(std::ostream& os, const capped_int& out) {
    return os << out.v;
  }
};
