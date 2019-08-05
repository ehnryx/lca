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
int main() {
    return 0;
}
