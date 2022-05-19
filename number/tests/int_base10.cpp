#include <bits/stdc++.h>
using namespace std;

//%:include "utility/fast_input.h"
#include "../int_base10.h"

using ll = long long;
using ld = long double;
using pt = complex<ld>;

constexpr char nl = '\n';
constexpr int MOD = 998244353;
constexpr ld EPS = 1e-9L;
random_device _rd; mt19937 rng(_rd());

using Int = int_base10;

void header() {
  cout << setw(5) << "/";
  cout << setw(5) << "%";
  cout << setw(5) << "*";
  cout << setw(5) << "+";
  cout << setw(5) << "-";
  cout << nl;
}

void f(Int a, Int b) {
  cout << setw(5) << a / b;
  cout << setw(5) << a % b;
  cout << setw(5) << a * b;
  cout << setw(5) << a + b;
  cout << setw(5) << a - b;
  cout << nl;
}

int main() {
  cin.tie(0)->sync_with_stdio(0);
  cout << fixed << setprecision(10);
#ifdef USING_FAST_INPUT
  fast_input cin;
#endif

  Int a = -17;
  Int b = 4;
  Int c = -5;
  Int d = 17;

  cout << "a = " << a << nl;
  cout << "b = " << b << nl;
  cout << "c = " << c << nl;
  cout << "d = " << d << nl;
  cout << nl;

  cout << "    "; header();
  cout << "a,b "; f(a, b);
  cout << "a,c "; f(a, c);
  cout << "b,c "; f(b, c);
  cout << "c,a "; f(c, a);
  cout << "d,b "; f(d, b);
  cout << "d,c "; f(d, c);
  cout << "b,d "; f(b, d);

  return 0;
}
