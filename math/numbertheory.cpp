#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 1e5+1; // some N

////////////////////////////////////////////////////////////////////////
// Linear Time Sieve for Multiplicative Functions -- O(n) (UNTESTED)
// f[n] stores the multiplicative function f(n)
// e[n] stores exp_p(n) where p is the smallest prime divisor of n
// Example with Euler's totient function
//
// USAGE:
//  1. set the commented lines to the correct multiplicative function
//  2. Sieve::run()
//  3. ???
//  4. profit
//*!
namespace Sieve {
    vector<int> primes;
    bool composite[N];
    int f[N], e[N];
    void run() {
        memset(composite, 0, sizeof composite);
        f[1] = 1; // input f(1)
        for (int i = 2; i < N; i++) {
            if (!composite[i]) {
                primes.push_back(i);
                f[i] = i-1; // input f(i) for prime i
                e[i] = 1;
            }
            for (int j : primes) {
                int v = i*j;
                if (v >= N) break;
                composite[v] = true;
                if (i % j == 0) {
                    // we may need f(ij) = f(i/j^e[i]) f(j^(e[i]+1))
                    f[v] = f[i] * j; // calc f(ij) when j divides i
                    e[v] = e[i] + 1;
                    break;
                } else {
                    f[v] = f[i] * f[j]; // calc f(ij) when gcd(i,j)=1
                    e[v] = 1;
                }
            }
        }
    }
}
//*/

////////////////////////////////////////////////////////////////////////
// Mobius Function Properties
//  0. multiplicative with mu(1) = 1 and mu(p^k) = -1 if k=0 else 0
//  1. sum_{d|n} mu(d) = 1 if m=1 else 0
//  2. #{x in [1,a] : gcd(b,x)=1} = sum_{d|b} mu(d) floor(a/d)

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
