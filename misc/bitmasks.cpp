#include <bits/stdc++.h>
using namespace std;

////////////////////////////////////////////////////////////////////////
// Iterate over subsets of subsets of 2^n -- O(3^n)
//*!
for (int bm=1; bm<1<<n; bm++) {
    // this excludes bm, use sm=bm to include bm
    for (int sm=(bm-1)&bm; sm>0; sm=(sm-1)&bm) {
        // sm is submask, bm is bitmask for subset of 2^n
        // do stuff here
    }
}
//*/

////////////////////////////////////////////////////////////////////////
// Next Permutation for bitmasks -- O(1)
// first permutation of k bits is (1<<k)-1
//*!
int next_bits(int v) {
    int t = (v|(v-1))+1;
    return t|((((t&-t)/(v&-v))>>1)-1);
}
//*/

////////////////////////////////////////////////////////////////////////
// Sum Over Subsets DP -- O(n2^n)
// calculates sum[x] = sum_{i subset of x} a[i]
//*!
void sosdp(int n, int a[], int sum[]) {
    for (int i = 0; i < 1<<n; i++) {
        sum[i] = a[i];
    }
    for (int i = 0; i < n; i++) {
        for (int bm = 0; bm < 1<<n; bm++) {
            if (bm & 1<<i) {
                sum[bm] += sum[bm ^ 1<<i];
            }
        }
    }
}
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
