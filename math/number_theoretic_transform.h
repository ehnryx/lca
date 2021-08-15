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
void number_theoretic_transform(vector<T>& a, const T& p2_root) {
  int n = (int)size(a); // n should be a power of 2
  int L = 31 - __builtin_clz(n); // logn
  static vector<T> root(2, 1);
  for (static int k = 2; k < n; k *= 2) {
    root.resize(n, 1);
    if constexpr (is_integral_v<T>) {
      T w = modpow<mod>(p2_root, mod / 2 / k);
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
  vector<int> rev(n);
  for (int i = 0; i < n; i++) {
    rev[i] = (rev[i >> 1] | (i & 1) << L) >> 1;
    if (i < rev[i]) {
      swap(a[i], a[rev[i]]);
    }
  }
  for (int k = 1; k < n; k *= 2) {
    for (int i = 0; i < n; i += 2 * k) {
      for (int j = 0; j < k; j++) {
        if constexpr (is_integral_v<T>) {
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

template <long long mod, typename T>
T find_ntt_root() {
  if constexpr (is_integral_v<T>) {
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

int find_ntt_length(int n) {
  return 1 << (n == 1 ? 0 : 32 - __builtin_clz(n - 1));
}

template <long long mod, typename T>
vector<T> convolve(vector<T> a, vector<T> b, size_t cut = -1) {
  static_assert(is_same_v<T, long long> || !is_integral_v<T>);
  if (empty(a) || empty(b)) return vector<T>();
  size_t length = size(a) + size(b) - 1;
  int n = find_ntt_length((int)length);
  T inv;
  if constexpr (is_integral_v<T>) inv = modpow<mod>(n, mod - 2);
  else inv = T(n).inverse();
  T p2_root = find_ntt_root<mod, T>();
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
    if constexpr (is_integral_v<T>) {
      a[i] = a[i] * b[i] % mod * inv % mod;
    } else {
      a[i] *= b[i];
      a[i] *= inv;
    }
  }
  reverse(begin(a) + 1, end(a));
  number_theoretic_transform<mod>(a, p2_root);
  a.resize(min(length, cut));
  return a;
}

