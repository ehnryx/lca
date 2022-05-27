/* Number Theoretic Transform
 * USAGE
 *  ab = convolve<P>(a, b); to multiply (sum_i a[i] x^i) * (sum_i b[i] x^i)
 * TIME
 *  O(NlogN)
 *  N = |array|
 * STATUS
 *  tested: kattis/kinversions
 */
#pragma once

#include <vector>

namespace ntt {
  template <long long mod>
  long long modpow(long long base, long long exponent) {
    long long res = 1;
    while (exponent != 0) {
      if (exponent & 1) {
        res = res * base % mod;
      }
      base = base * base % mod;
      exponent >>= 1;
    }
    return res;
  }

  template <long long mod, typename T>
  T get_inverse(T v) {
    if constexpr (std::is_integral_v<T>) {
      return modpow<mod>(v, mod - 2);
    } else {
      return v.inverse();
    }
  }
}

template <long long mod, typename T>
void number_theoretic_transform(std::vector<T>& a, const T& p2_root) {
  int n = (int)a.size(); // n should be a power of 2
  int L = 31 - __builtin_clz(n); // logn
  static std::vector<T> root(2, 1);
  for (static int k = 2; k < n; k *= 2) {
    root.resize(n, 1);
    if constexpr (std::is_integral_v<T>) {
      T w = ntt::modpow<mod>(p2_root, mod / 2 / k);
      for (int i = k; i < 2 * k; i++) {
        root[i] = (i % 2 ? root[i / 2] * w % mod : root[i / 2]);
      }
    } else {
      T w = p2_root.pow(mod / 2 / k);
      for (int i = k; i < 2 * k; i++) {
        root[i] = (i % 2 ? root[i / 2] * w : root[i / 2]);
      }
    }
  }
  std::vector<int> rev(n);
  for (int i = 0; i < n; i++) {
    rev[i] = (rev[i >> 1] | (i & 1) << L) >> 1;
    if (i < rev[i]) {
      swap(a[i], a[rev[i]]);
    }
  }
  for (int k = 1; k < n; k *= 2) {
    for (int i = 0; i < n; i += 2 * k) {
      for (int j = 0; j < k; j++) {
        if constexpr (std::is_integral_v<T>) {
          T z = root[j + k] * a[i + j + k] % mod;
          a[i + j + k] = (a[i + j] - z < 0 ? a[i + j] - z + mod : a[i + j] - z);
          a[i + j] = (a[i + j] + z < mod ? a[i + j] + z : a[i + j] + z - mod);
        } else {
          T z = root[j + k] * a[i + j + k];
          a[i + j + k] = a[i + j] - z;
          a[i + j] += z;
        }
      }
    }
  }
}

namespace ntt {
  template <long long mod, typename T>
  T find_root() {
    if constexpr (std::is_integral_v<T>) {
      T p2_root = 2;
      while (modpow<mod>(p2_root, mod / 2) == 1) {
        p2_root += 1;
      }
      return p2_root;
    } else {
      T p2_root(2);
      while (p2_root.pow(mod / 2) == 1) {
        p2_root += 1;
      }
      return p2_root;
    }
  }

  int find_length(int n) {
    return 1 << (n == 1 ? 0 : 32 - __builtin_clz(n - 1));
  }

  template <long long mod, typename T>
  std::vector<T> convolve(std::vector<T> a, std::vector<T> b, size_t cut = -1) {
    static_assert(std::is_same_v<T, long long> || !std::is_integral_v<T>);
    if (empty(a) || empty(b)) return std::vector<T>();
    size_t length = a.size() + b.size() - 1;
    int n = find_length((int)length);
    T inv = get_inverse<mod>(T(n));
    T p2_root = find_root<mod, T>();
    bool equal = (a == b);
    a.resize(n);
    number_theoretic_transform<mod>(a, p2_root);
    if (equal) {
      b = a;
    } else {
      b.resize(n);
      number_theoretic_transform<mod>(b, p2_root);
    }
    for (int i = 0; i < n; i++) {
      if constexpr (std::is_integral_v<T>) {
        a[i] = a[i] * b[i] % mod * inv % mod;
      } else {
        a[i] *= b[i] * inv;
      }
    }
    reverse(begin(a) + 1, end(a));
    number_theoretic_transform<mod>(a, p2_root);
    a.resize(std::min(length, cut));
    return a;
  }
}

