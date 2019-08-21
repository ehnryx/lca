#include <bits/stdc++.h>
using namespace std;

////////////////////////////////////////////////////////////////////////
// Wavelet Tree (UNTESTED) -- Queries are O(log(A))
// No updates are allowed! Everything is 1-indexed
// USAGE:
//  0. WaveletTree wt(s.begin(), s.end()); WARNING: s gets destroyed!
//*!
struct WaveletTree {
    int lo, hi;
    WaveletTree *left, *right;
    vector<int> b;
    template<class T> WaveletTree(T s, T e):
        WaveletTree(s, e, *min_element(s,e), *max_element(s,e)) {}
    template<class T> WaveletTree(T s, T e, int l, int r): lo(l), hi(r) {
        if (lo == hi || s == e) { left = right = nullptr; return; }
        int mid = (lo+hi)/2;
        auto f = [&](int x) { return x <= mid; };
        b.reserve(distance(s,e)+1); b.push_back(0);
        for (auto it=s; it!=e; it++) b.push_back(b.back()+f(*it));
        auto m = stable_partition(s, e, f);
        left = new WaveletTree(s, m, lo, mid);
        right = new WaveletTree(m, e, mid+1, hi);
    }

    // nth smallest element, inclusive range, must be valid
    int nth(int l, int r, int n) {
        if (l > r) return INF;
        if (lo == hi) return lo;
        int lb = b[l-1], rb = b[r];
        if (n <= rb-lb) return left->nth(lb+1, rb, n);
        else return right->nth(l-lb, r-rb, n-rb+lb);
    }

    // number of elements less than or equal to k
    int leq(int l, int r, int k) {
        if (l > r || k < lo) return 0;
        if (hi <= k) return r-l+1;
        int lb = b[l-1], rb = b[r];
        return left->leq(lb+1, rb, k) + right->leq(l-lb, r-rb, k);
    }

    int count(int l, int r, int k) {
        if (l > r || k < lo || k > hi) return 0;
        if (lo == hi) return r-l+1;
        int lb = b[l-1], rb = b[r], m = (lo+hi)/2;
        if (k <= m) return left->count(lb+1, rb, k);
        else return right->count(l-lb, r-rb, k);
    }
};
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
