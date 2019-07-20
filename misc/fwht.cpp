#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int,int> pii;

const int INF = 0x3f3f3f3f;

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
        For(i,N) {
            p[i] = p[i] * invN;
        }
    }
}
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
