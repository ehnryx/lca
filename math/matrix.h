/* Matrix (for Gaussian elimination)
 * USAGE
 *  matrix<T> f(vector<vector<T>>>);  or  matrix<T> f(n, m);
 * TIME
 *  O(?)
 * STATUS
 *  tested: kattis/dimensionalanalysis
 */
#pragma once

#include <iomanip>
#include <stdexcept>
#include <vector>

template <typename T>
struct matrix : std::vector<std::vector<T>> {
  using std::vector<std::vector<T>>::data, std::vector<std::vector<T>>::at;
  using std::vector<std::vector<T>>::size, std::vector<std::vector<T>>::empty;

  std::vector<bool> is_free;
  std::vector<T> ans;
  int n, m;

  matrix() : std::vector<std::vector<T>>() {}
  matrix(int _n, int _m) : std::vector<std::vector<T>>(_n, std::vector<T>(_m)) {}
  matrix(const std::vector<std::vector<T>>& v) : std::vector<std::vector<T>>(v) {
    check_dimensions();
  }
  matrix(std::vector<std::vector<T>>&& v) : std::vector<std::vector<T>>(move(v)) {
    check_dimensions();
  }
  void check_dimensions() {
    for (size_t i = 1; i < size(); i++) {
      if (data()[i].size() != data()[i - 1].size()) {
        throw std::invalid_argument("matrix rows must have the same size");
      }
    }
  }

  // returns rank
  int rref() {
    if (empty()) return 0;
    check_dimensions();
    n = (int)size();
    m = (int)at(0).size();
    int rank = 0;
    for (int j = 0; j < m; j++) {
      int pivot = -1;
      for (int i = rank; i < n; i++) {
        if (data()[i][j] != 0) {
          pivot = i;
          break;
        }
      }
      if (pivot == -1) {
        continue;
      }
      swap(data()[rank], data()[pivot]);
      for (int i = 0; i < n; i++) {
        if (i == rank || data()[i][j] == 0) continue;
        T mul = data()[i][j] / data()[rank][j];
        for (int k = j; k < m; k++) {
          data()[i][k] -= data()[rank][k] * mul;
        }
        if (data()[i][j] != 0) {
          throw std::runtime_error("could not zero out value with pivot");
        }
      }
      rank++;
    }
    return rank;
  }

  int solve() {
    if (empty()) return 0;
    int rank = rref();
    is_free.resize(m - 1, true);
    ans.resize(m - 1);
    for (int i = 0; i < rank; i++) {
      int pivot = -1;
      for (int j = 0; j < m; j++) {
        if (data()[i][j] != 0) {
          pivot = j;
          break;
        }
      }
      if (pivot == m - 1) {
        return -1;  // could not solve!!
      }
      if (pivot == -1) {
        throw std::runtime_error("could not find pivot");
      }
      ans[pivot] = data()[i].back() / data()[i][pivot];
      is_free[pivot] = false;
    }
    return rank;
  }

  friend std::ostream& operator<<(std::ostream& os, const matrix& mat) {
    if (mat.empty()) return os << "[]";
    std::vector<size_t> col_width(mat[0].size());
    for (const std::vector<T>& row : mat) {
      for (size_t i = 0; i < row.size(); i++) {
        std::ostringstream test_os;
        test_os << row[i];
        col_width[i] = std::max(col_width[i], test_os.str().size());
      }
    }
    os << "[\n";
    for (const std::vector<T>& row : mat) {
      for (size_t i = 0; i < row.size(); i++) {
        os << std::setw((int)col_width[i] + 2) << row[i];
      }
      os << "\n";
    }
    return os << "]";
  }
};
