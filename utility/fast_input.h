/* Fast Input
 *  fast_input<buf_size> cin;
 *  cin >> blah;
 *  array = cin<T, pad>(n);
 *  grid = cin<T, pad, pad>(n, m);
 *  prism = cin<T, pad, pad, pad>(n, m, h);
 * STATUS
 *  tested ?
 */
#pragma once

#include "fast_input_read.h"
#include "member_function_checker.h"

#include <complex>
#include <string>
#include <tuple>
#include <vector>

#define USING_FAST_INPUT

template <size_t buf_size = 10000>
struct fast_input {
  char buf[buf_size], *S, *T, c = 0;
  int positive;
  FILE* ifptr;
  fast_input(FILE* _in = stdin) : S(buf), T(buf), ifptr(_in) {}

  explicit operator bool() const { return c != EOF; }

  inline char getc(void) {
    if (c == EOF) return EOF;
    if (S == T) {
      T = (S = buf) + fread(buf, 1, buf_size, ifptr);
      if (S == T) return c = EOF;
    }
    return *S++;
  }

  inline void get(char& x) {
    while (isspace(x = getc()) && x != EOF)
      ;
  }

  inline void get(std::string& x) {
    x.clear();
    while (isspace(c = getc()) && c != EOF)
      ;
    for (; !isspace(c) && c != EOF; c = getc()) {
      x.push_back(c);
    }
  }

  inline void get(decltype(std::ignore)) {
    while (isspace(c = getc()) && c != EOF)
      ;
    for (; !isspace(c) && c != EOF; c = getc())
      ;
  }

  template <typename var_t>
    requires(std::is_integral_v<var_t>)
  inline void get(var_t& x) {
    x = 0;
    positive = 1;
    while (!isdigit(c = getc()) && c != EOF) {
      positive = (c ^ '-');
    }
    for (; isdigit(c) && c != EOF; c = getc()) {
      x = x * 10 + c - '0';
    }
    if (!positive) {
      x = -x;
    }
  }

  template <typename var_t>
    requires(std::is_floating_point_v<var_t>)
  inline void get(var_t& x) {
    x = 0;
    positive = 1;
    while (!isdigit(c = getc()) && c != '.' && c != EOF) {
      positive = (c ^ '-');
    }
    if (c != '.') {
      for (; isdigit(c) && c != EOF; c = getc()) {
        x = x * 10 + c - '0';
      }
    }
    if (c == '.') {
      static var_t div;
      div = 1;
      while (isdigit(c = getc()) && c != EOF) {
        x = x * 10 + c - '0';
        div *= 10;
      }
      x /= div;
    }
    if (!positive) {
      x = -x;
    }
  }

  template <typename T>
  inline void get(std::complex<T>& x) {
    x.real(getv<T>());
    x.imag(getv<T>());
  }

  template <typename T, typename U>
  inline void get(std::pair<T, U>& x) {
    *this >> x.first >> x.second;
  }

  template <size_t index = 0, typename... T>
  inline void get(std::tuple<T...>& x) {
    if constexpr (index < sizeof...(T)) {
      *this >> (std::get<index>(x));
      get<index + 1>(x);
    }
  }

  MEMBER_FUNCTION_CHECKER(get);

  template <typename T>
  inline fast_input& operator>>(T& x) {
    if constexpr (_has_get<fast_input, T&>::value) {
      get(x);
    } else {
      fast_input_read<fast_input<buf_size>, T>::get(*this, x);
    }
    return *this;
  }

  std::string getline() {
    std::string out;
    while ((c = getc()) != '\n' && c != EOF) {
      out.push_back(c);
    }
    return out;
  }

  std::vector<std::string> readlines(int n) {
    std::vector<std::string> out;
    out.reserve(n);
    for (int i = 0; i < n; i++) {
      out.push_back(getline());
    }
    return out;
  }

  template <typename T>
  T getv() {
    T x;
    *this >> x;
    return x;
  }

  template <typename... T>
    requires(sizeof...(T) != 1)
  std::tuple<T...> getv() {
    return getv<std::tuple<T...>>();
  }

  template <typename T, int pad_n = 0>
  std::vector<T> read(int n) {
    return readinit<T, pad_n>(n, T());
  }

  template <typename T, int pad_n = 0, int pad_m = 0>
  std::vector<std::vector<T>> read(int n, int m) {
    return readinit<T, pad_n, pad_m>(n, m, T());
  }

  template <typename T, int pad_n = 0, int pad_m = 0, int pad_h = 0>
  std::vector<std::vector<std::vector<T>>> read(int n, int m, int h) {
    return readinit<T, pad_n, pad_m, pad_h>(n, m, h, T());
  }

  template <typename T, int pad_n = 0>
  std::vector<T> readinit(int n, T init) {
    std::vector<T> out(n + 2 * pad_n, init);
    for (int i = 0; i < n; i++) {
      *this >> out[pad_n + i];
    }
    return out;
  }

  template <typename T, int pad_n = 0, int pad_m = 0>
  std::vector<std::vector<T>> readinit(int n, int m, T init) {
    std::vector out(n + 2 * pad_n, std::vector<T>(m + 2 * pad_m, init));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        *this >> out[pad_n + i][pad_m + j];
      }
    }
    return out;
  }

  template <typename T, int pad_n = 0, int pad_m = 0, int pad_h = 0>
  std::vector<std::vector<std::vector<T>>> readinit(int n, int m, int h, T init) {
    std::vector out(
        n + 2 * pad_n, std::vector(m + 2 * pad_m, std::vector<T>(h + 2 * pad_h, init)));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        for (int k = 0; k < h; k++) {
          *this >> out[pad_n + i][pad_m + j][pad_h + k];
        }
      }
    }
    return out;
  }
};
