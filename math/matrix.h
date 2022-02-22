/* Matrix (for Gaussian elimination)
 * USAGE
 *  matrix<T> f(vector<vector<T>>>);  or  matrix<T> f(n, m);
 * TIME
 *  O(?)
 * STATUS
 *  tested: kattis/dimensionalanalysis
 */
#pragma once

template <typename T>
struct matrix : vector<vector<T>> {
  using vector<vector<T>>::data, vector<vector<T>>::at;
  using vector<vector<T>>::size, vector<vector<T>>::empty;
  matrix(): vector<vector<T>>() {}
  matrix(int n, int m): vector<vector<T>>(n, vector<T>(m)) {}
  matrix(const vector<vector<T>>& v): vector<vector<T>>(v) {
    check_dimensions();
  }
  matrix(vector<vector<T>>&& v): vector<vector<T>>(forward(v)) {
    check_dimensions();
  }
  void check_dimensions() {
    for (size_t i = 1; i < size(); i++) {
      if (data()[i].size() != data()[i-1].size()) {
        throw invalid_argument("matrix rows must have the same size");
      }
    }
  }

  // returns rank
  int rref() {
    if (empty()) return 0;
    int n = (int)size();
    int m = (int)at(0).size();
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
        assert(data()[i][j] == 0);
      }
      rank++;
    }
    return rank;
  }

  friend ostream& operator << (ostream& os, const matrix& mat) {
    if (mat.empty()) return os << "[]";
    vector<size_t> col_width(mat[0].size());
    for (const vector<T>& row : mat) {
      for (size_t i = 0; i < row.size(); i++) {
        ostringstream test_os;
        test_os << row[i];
        col_width[i] = max(col_width[i], test_os.str().size());
      }
    }
    os << "[\n";
    for (const vector<T>& row : mat) {
      for (size_t i = 0; i < row.size(); i++) {
        os << setw((int)col_width[i] + 2) << row[i];
      }
      os << "\n";
    }
    return os << "]";
  }
};

