#include <bits/stdc++.h>
using namespace std;

// for printing
#define nl '\n'

/**
 * Matrix struct for square matrices
 * TESTED ON
 * multiplication:
 *  kattis: tritiling
 *  cf: 222E
 * row reduction:
 *  kattis: arcade, primonimo
 *  swerc16: i
 */

// 125 lines
/**
 * USAGE:
 *  to declare an nxn diagonal matrix: Matrix<T> mat(n,n,d);
 *  to declare a rxc matrix: Matrix<T> mat(r,c);
 *  can convert vector<vector<T>> to Matrix<T> implicitly;
 *
 *  to solve a system of equations: Ax = b
 *  define Matrix<T> A;
 *  then x = A.solve(b) where x and b are vectors
 *  free variables are all arbitrarily set to 0
 *  returns an empty vector if no solution
 */
template <class T> struct Matrix {
	int rows, cols;
	vector<vector<T>> a;
	Matrix(int r, int c, int v=0) {
		rows = r; cols = c;
		a.resize(r, vector<T>(c));
		if (v) for (int i = 0; i < min(r,c); i++)
			a[i][i] = v;
	}
	Matrix(const vector<vector<T>>& v) {
		rows = v.size(); cols = v[0].size();
		a = v;
	}
	vector<T>& operator [] (unsigned int i) {
		return a[i];
	}
	Matrix<T>& operator *= (const Matrix<T>& other) {
		return *this = *this * other;
	}
	Matrix<T> operator * (const Matrix<T>& other) {
		Matrix<T> res(rows,cols);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				res[i][j] = 0;
				for (int k = 0; k < cols; k++)
					res[i][j] += a[i][k] * other.a[k][j];
			}
		}
		return res;
	}
	vector<T> operator * (const vector<T>& v) {
		vector<T> res(rows);
		for (int i = 0; i < rows; i++) {
			res[i] = 0;
			for (int j = 0; j < cols; j++)
				res[i] += a[i][j] * v[j];
		}
		return res;
	}
	Matrix<T>& operator += (const Matrix<T>& other) {
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				a[i][j] += other.a[i][j];
		return *this;
	}
	vector<T> solve(vector<T> goal) {
		vector<int> piv, first; // pivot and first
		vector<T> res; // the solution
		Matrix<T> rdu(rows, cols); // reduced matrix
		for (int i = 0; i < rows; i++) {
			first.push_back(cols);
			for (int j = 0; j < cols; j++) {
				rdu[i][j] = a[i][j];
				if (a[i][j] != 0 && first[i] == cols)
					first[i] = j;
			}
		}
		for (int i = 0; i < rows; i++) {
			piv.push_back(0);
			for (int j = 0; j < rows; j++) {
				if (i == 0 || first[piv[i-1]] < first[j]) {
					piv[i] = j;
					break;
				}
			}
			for (int j = piv[i]+1; j < rows; j++)
				if (i == 0 || first[piv[i-1]] < first[j])
					if (first[j] < first[piv[i]])
						piv[i] = j;
			if (first[piv[i]] == cols) {
				piv.pop_back();
				break;
			}
			for (int j = 0; j < rows; j++) {
				if (j != piv[i] && first[j] == first[piv[i]]) {
					bool zero = true;
					T factor = rdu[j][first[piv[i]]] / rdu[piv[i]][first[piv[i]]];
					goal[j] -= factor * goal[piv[i]];
					for (int k = first[piv[i]]; k < cols; k++) {
						rdu[j][k] -= factor * rdu[piv[i]][k];
						if (abs(rdu[j][k]) <= EPS)
							rdu[j][k] = 0;
						if (rdu[j][k] == 0 && zero)
							first[j]++;
						else if (rdu[j][k] != 0)
							zero = false;
					}
					if (zero && goal[j] != 0)
						return res;
				}
			}
		}
		res.resize(rows, 0);
		int pos = cols-1;
		while (piv.empty() == false) {
			int curr = piv.back();
			piv.pop_back();
			while (first[curr] < pos)
				pos--;
			res[pos] = goal[curr];
			for (int j = pos+1; j < cols; j++)
				res[pos] -= res[j] * rdu[curr][j];
			res[pos] /= rdu[curr][pos];
			pos--;
		}
		return res;
	}
};
