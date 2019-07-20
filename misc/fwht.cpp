#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int,int> pii;

// value works for 1e5. N must be a power of 2
const int N = 1<<17;

////////////////////////////////////////////////////////////////////////
// Fast Walsh Hadamard Transform -- O(nlog(n))
// TESTED on escom
// Essentially FFT with xor instead of addition
// USAGE:
//  1. fwht(a); fwht(b);
//  2. for i in 0..N-1: ans[i] = a[i] * b[i]
//  3. fwht(ans, true);
//*!
template<class T> void fwht(T p[], bool inv = false) {
    for (int len=1; 2*len<=N; len*=2) {
        for (int i=0; i<N; i+=2*len) {
            for (int j=0; j<len; j++) {
                T u = p[i+j];
                T v = p[i+len+j];
                p[i+j] = u+v;
                p[i+len+j] = u-v;
            }
        }
    }
    if (inv) {
        for (int i=0; i<N; i++) {
            p[i] = p[i] / N;
        }
    }
}
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
