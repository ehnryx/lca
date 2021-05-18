/* Square Matrix
 * USAGE
 *  square_matrix<T> mat(dimension, value_on_diagonal);
 * STATUS
 *  mostly untested
 */
#pragma once

template <class T, int N>
struct square_matrix {
  static constexpr int n = N;
  array<array<T, N>, N> data;
  square_matrix(const T& v = 0) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        data[i][j] = (i == j ? v : T(0));
      }
    }
  }
  square_matrix(const vector<vector<T>>& v) {
    assert((int)v.size() == n);
    for (int i = 0; i < n; i++) {
      assert((int)v[i].size() == n);
      for (int j = 0; j < n; j++) {
        data[i][j] = v[i][j];
      }
    }
  }
  array<T, N>& operator [] (int i) { return data[i]; }
  const array<T, N>& operator [] (int i) const { return data[i]; }
  int size() const { return n; }
  square_matrix operator + (const square_matrix& o) const { return square_matrix(*this) += o; }
  square_matrix operator - (const square_matrix& o) const { return square_matrix(*this) -= o; }
  square_matrix operator * (const square_matrix& o) const { return square_matrix(*this) *= o; }
  square_matrix& operator += (const square_matrix& o) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        data[i][j] += o[i][j];
      }
    }
    return *this;
  }
  square_matrix& operator -= (const square_matrix& o) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        data[i][j] -= o[i][j];
      }
    }
    return *this;
  }
  square_matrix& operator *= (const square_matrix& o) {
    array<array<T, N>, N> res;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        res[i][j] = 0;
        for (int k = 0; k < n; k++) {
          res[i][j] += data[i][k] * o[k][j];
        }
      }
    }
    data = move(res);
    return *this;
  }
  template <typename exp_t, typename = enable_if_t<is_integral_v<exp_t>>>
  square_matrix pow(exp_t exponent) const {
    square_matrix res(n, 1), base(*this);
    for ( ; exponent > 0; exponent /= 2) {
      if (exponent % 2) res *= base;
      base *= base;
    }
    return res;
  }
  friend ostream& operator << (ostream& os, const square_matrix& mat) {
    os << '[';
    for (int i = 0; i < mat.n; i++) {
      if (i > 0) os << ", ";
      os << '[';
      for (int j = 0; j < mat.n; j++) {
        if (j > 0) os << ", ";
        os << mat[i][j];
      }
      os << ']';
    }
    return os << ']';
  }
};

