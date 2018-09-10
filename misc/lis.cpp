#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int,int> pii;

const int INF = 0x3f3f3f3f;

////////////////////////////////////////////////////////////////////////
// Longest Increasing Subsequence -- O(nlog(n)) (UNTESTED)
// Finds the longest increasing subsequence
// Returns the length, stores the answer in lis
//*!
int get_lis(const vector<int>& a, vector<int>& lis) {
    int n = a.size();
    int pred[n];
    vector<pii> vals;
    for (int i = 0; i < n; i++) {
        auto it = lower_bound(vals.begin(), vals.end(), pii(a[i], 0));
        if (it == vals.begin()) {
            pred[i] = -1;
        } else {
            pred[i] = prev(it)->second;
        }
        if (it == vals.end()) {
            vals.push_back(pii(a[i], i));
        } else {
            *it = pii(a[i], i);
        }
    }
    lis.clear();
    for (int i = vals.back().second; i != -1; i = pred[i]) {
        lis.push_back(a[i]); // we can also get the indices instead
    }
    reverse(lis.begin(), lis.end());
    return vals.size();
}
//*/

////////////////////////////////////////////////////////////////////////
// Longest Increasing Subsequence Length -- O(nlog(n)) (UNTESTED)
// Returns the length of a longest increasing subsequence
//*!
int lis_len(const vector<int>& a) {
    int n = a.size();
    int lis[n];
    memset(lis, INF, sizeof lis);
    for (int i = 0; i < n; i++) {
        *lower_bound(lis, lis+n, a[i]) = a[i];
    }
    return lower_bound(lis, lis+n, INF) - lis;
}
//*/

////////////////////////////////////////////////////////////////////////
// Weighted Longest Increasing Subsequence Length -- O(nlog(n)) (TESTED)
// Returns the weight of a weighted longest increasing subsequence
// pii: value, weight
//*!
ll weighted_lis_len(const vector<pair<int,ll>>& a) {
    int n = a.size();
    map<int,ll> lis;
    for (int i = 0; i < n; i++) {
        auto it = lis.lower_bound(a[i].first);
        ll cur = a[i].second;
        while (cur > 0 && it != lis.end()) {
            if (it->second <= cur) {
                cur -= it->second;
                it = lis.erase(it);
            } else {
                it->second -= cur;
                cur = 0;
            }
        }
        lis[a[i].first] += a[i].second;
    }
    ll len = 0;
    for (const auto& it : lis) {
        len += it.second;
    }
    return len;
}
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
