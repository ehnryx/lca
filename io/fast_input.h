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

#include "../misc/member_function_checker.h"

#include <complex>
#include <string>
#include <vector>

#define USING_FAST_INPUT

template <int buf_size = 10000>
struct fast_input {
  char buf[buf_size], *S, *T, c = 0;
  int positive;
  FILE* ifptr;
  fast_input(FILE* _in = stdin): S(buf), T(buf), ifptr(_in) {}

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
    while (isspace(x = getc()) && x != EOF);
  }

  inline void get(std::string& x) {
    x.clear();
    while (isspace(c = getc()) && c != EOF);
    for (; !isspace(c) && c != EOF; c = getc()) {
      x.push_back(c);
    }
  }

  template <typename var_t, std::enable_if_t<std::is_integral_v<var_t>, bool> = true>
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

  template <typename var_t, std::enable_if_t<std::is_floating_point_v<var_t>, bool> = true>
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

  MEMBER_FUNCTION_CHECKER(get);

  template <typename T>
  inline fast_input& operator >> (T& x) {
    if constexpr (_has_get<fast_input, T&>::value) {
      get(x);
    } else {
      x.fast_read(*this);
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

  template <typename T>
  T getv() {
    T x;
    *this >> x;
    return x;
  }

  template <typename T, int pad_n = 0, T init = T()>
  std::vector<T> read(int n) {
    std::vector<T> out(n + 2 * pad_n, init);
    for (int i = 0; i < n; i++) {
      *this >> out[pad_n + i];
    }
    return out;
  }

  template <typename T, int pad_n = 0, int pad_m = 0, T init = T()>
  std::vector<std::vector<T>> read(int n, int m) {
    std::vector out(n + 2 * pad_n, std::vector<T>(m + 2 * pad_m, init));
    for (int i = 0; i < n; i++) {
      for(int j = 0; j < m; j++) {
        *this >> out[pad_n + i][pad_m + j];
      }
    }
    return out;
  }

  template <typename T, int pad_n = 0, int pad_m = 0, int pad_h = 0, T init = T()>
  std::vector<std::vector<std::vector<T>>> read(int n, int m, int h) {
    std::vector out(n + 2 * pad_n, std::vector(m + 2 * pad_m,
          std::vector<T>(h + 2 * pad_h, init)));
    for (int i = 0; i < n; i++) {
      for(int j = 0; j < m; j++) {
        for (int k = 0; k < h; k++) {
          *this >> out[pad_n + i][pad_m + j][pad_h + k];
        }
      }
    }
    return out;
  }
};

