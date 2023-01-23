/* Square Matrix
 * USAGE
 *  square_matrix_var<T> mat(dimension, value_on_diagonal);
 * STATUS
 *  mostly untested
 */
#pragma once

template <typename T>
struct square_matrix_var {
  const int n;
  vector<vector<T>> data;
  square_matrix_var(int _n, const T& v = 0): n(_n), data(n, vector<T>(n)) {
    for (int i = 0; i < n; i++) {
      data[i][i] = v;
    }
  }
  square_matrix_var(const vector<vector<T>>& v): n(v.size()), data(v) {
    assert((int)v.size() == n);
    for (int i = 0; i < n; i++) {
      assert((int)v[i].size() == n);
    }
  }
  vector<T>& operator [] (int i) { return data[i]; }
  const vector<T>& operator [] (int i) const { return data[i]; }
  int size() const { return n; }
  square_matrix_var operator + (const square_matrix_var& o) const { return square_matrix_var(*this) += o; }
  square_matrix_var operator - (const square_matrix_var& o) const { return square_matrix_var(*this) -= o; }
  square_matrix_var operator * (const square_matrix_var& o) const { return square_matrix_var(*this) *= o; }
  square_matrix_var& operator += (const square_matrix_var& o) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        data[i][j] += o[i][j];
      }
    }
    return *this;
  }
  square_matrix_var& operator -= (const square_matrix_var& o) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        data[i][j] -= o[i][j];
      }
    }
    return *this;
  }
  square_matrix_var& operator *= (const square_matrix_var& o) {
    vector res(n, vector<T>(n));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
          res[i][j] += data[i][k] * o[k][j];
        }
      }
    }
    data = move(res);
    return *this;
  }
  template <typename exp_t, typename = enable_if_t<is_integral_v<exp_t>>>
  square_matrix_var pow(exp_t exponent) const {
    square_matrix_var res(n, 1), base(*this);
    for ( ; exponent > 0; exponent /= 2) {
      if (exponent % 2) res *= base;
      base *= base;
    }
    return res;
  }
  vector<T> operator * (const vector<T>& v) const {
    vector<T> res(n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        res[i] += data[i][j] * v[j];
      }
    }
    return res;
  }
  friend ostream& operator << (ostream& os, const square_matrix_var& mat) {
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

