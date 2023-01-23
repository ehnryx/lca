/* Fast Fourier Transform
 * http://neerc.ifmo.ru/trains/toulouse/2017/fft2.pdf
 * USAGE
 *  ab = convolve(a, b); to multiply (sum_i a[i] x^i) * (sum_i b[i] x^i)
 * TIME
 *  O(NlogN)
 *  N = |array|
 * STATUS
 *  untested
 */
#pragma once

#include <complex>
#include <vector>

template <typename D>
void fast_fourier_transform(std::vector<std::complex<D>>& a) {
  int n = (int)size(a); // n should be a power of 2
  int L = 31 - __builtin_clz(n); // logn
#ifdef _USE_MATH_DEFINES
  static constexpr D PI = D(M_PIl);
#else
  static constexpr D PI = acos(D(-1));
#endif
  static std::vector<std::complex<D>> root(2, 1);
  for (static int k = 2; k < n; k *= 2) {
    root.resize(n, 1);
    std::complex<D> w = std::polar(D(1), PI / k);
    for (int i = k; i < 2 * k; i++) {
      root[i] = (i % 2 ? root[i / 2] * w : root[i / 2]);
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
        std::complex<D> z = root[j + k] * a[i + j + k];
        a[i + j + k] = a[i + j] - z;
        a[i + j] += z;
      }
    }
  }
}

namespace fft {
  template <typename D = long double, typename Prod_t = void, typename T>
  auto convolve(const std::vector<T>& a, const std::vector<T>& b, size_t cut = -1) {
    static_assert(std::is_floating_point_v<D>);
    using Out_t = std::conditional_t<std::is_same_v<Prod_t, void>, T, Prod_t>;
    if (empty(a) || empty(b)) return std::vector<Out_t>();
    std::vector<Out_t> res(size(a) + size(b) - 1, 0);
    int L = (size(res) == 1 ? 0 : 32 - __builtin_clz(size(res) - 1));
    int n = 1 << L;
    std::vector<std::complex<D>> in(n), out(n);
    copy(begin(a), end(a), begin(in));
    for (size_t i = 0; i < size(b); i++) {
      in[i] += complex<D>(0, 1) * complex<D>(b[i]);
    }
    fast_fourier_transform(in);
    for (int i = 0; i < n; i++) {
      int j = (n - i) & (n - 1);
      out[i] = in[j] * in[j] - conj(in[i] * in[i]);
    }
    fast_fourier_transform(out);
    for (size_t i = 0; i < size(res); i++) {
      if constexpr (std::is_integral_v<Out_t>) {
        res[i] = Out_t(round(imag(out[i]) / (4 * n)));
      } else {
        res[i] = imag(out[i]) / (4 * n);
      }
    }
    if (cut < size(res)) {
      res.resize(cut);
    }
    return res;
  }
  template <typename T>
  auto convolve(std::vector<complex<T>> a, std::vector<complex<T>> b, size_t cut = -1) {
    if (empty(a) || empty(b)) return std::vector<complex<T>>();
    size_t length = a.size() + b.size() - 1;
    int n = 1 << (length == 1 ? 0 : 32 - __builtin_clz(length - 1));
    bool equal = (a == b);
    a.resize(n);
    fast_fourier_transform(a);
    if (equal) {
      b = a;
    } else {
      b.resize(n);
      fast_fourier_transform(b);
    }
    const T inv = 1 / T(n);
    for (int i = 0; i < n; i++) {
      a[i] *= b[i] * inv;
    }
    reverse(begin(a) + 1, end(a));
    fast_fourier_transform(a);
    a.resize(std::min(length, cut));
    return a;
  }
}

