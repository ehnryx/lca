#include "../queries/segtree.cpp"
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

////////////////////////////////////////////////////////////////////////
// Rectangle Union -- O(nlog(n)) (TESTED ONCE rather unconvincingly)
// Finds the area of the union of axis-aligned rectangles
// Allows storing of prefix areas up to specific points, for prefix sums
// USAGE:
//  1. add_rect(x1,y1,x2,y2); with x1<x2 and y1<y2
//   This defines the rectangle [x1,x2] x [y1,y2]
//  2. add_query(l,r); with l<r. Query is for the area with l<x<r
//  3. solve(); to compute the areas, should only be called ONCE
//  4. query(l,r); to query
//*!
struct RectangleUnion {
    struct Segment {
        int a, b, v;
        Segment(): v(0) {}
        Segment(int a, int b, int v): a(a), b(b), v(v) {}
    };
    map<ll,vector<Segment>> segs;
    map<ll,int> cmpr;
    vector<ll> decmp;
    unordered_map<ll,ll> area;
    vector<ll> tag, cover, length;

    void add_rect(ll a, ll b, ll x, ll y) {
        segs[a].push_back(Segment(b,y,1));
        segs[x].push_back(Segment(b,y,-1));
        cmpr[b]; cmpr[y];
    }
    void add_query(ll a, ll b) {
        segs[a].push_back(Segment());
        segs[b].push_back(Segment());
    }
    ll query(ll a, ll b) {
        if (!area.count(a) || !area.count(b)) {
            throw runtime_error("query not added");
        }
        return area[b] - area[a];
    }

    int n;
    void setup() {
        int cnt = 0;
        vector<ll> len;
        for (auto it = cmpr.begin(); it != cmpr.end(); it++) {
            if (next(it) == cmpr.end()) len.push_back(0);
            else len.push_back(next(it)->first - it->first);
            decmp.push_back(it->first);
            it->second = cnt++;
        }
        n = 1 << (32 - __builtin_clz(cnt-1));
        tag.resize(2*n, 0);
        cover.resize(2*n, 0);
        length.resize(2*n, 0);
        for (int i = 0; i < cnt; i++) 
            length[i+n] = len[i];
        for (int i = n-1; i > 0; i--) 
            length[i] = length[2*i] + length[2*i+1];
    }

    void solve() {
        setup();
        ll cur = 0;
        ll pre = 0x5F3759DF;
        for (const pair<int,vector<Segment>>& it : segs) {
            cur += cover[1] * (it.first - pre);
            pre = it.first;
            for (const Segment& s : it.second) {
                if (s.v == 0) area[it.first] = cur;
                else update(cmpr[s.a], cmpr[s.b]-1, s.v, 1, 0, n-1);
            }
        }
    }

    void update(int l, int r, int v, int i, int s, int e) {
        if (e < l || r < s) return;
        if (l <= s && e <= r) {
            tag[i] += v;
        } else {
            int mid = (s+e)/2;
            update(l, r, v, 2*i, s, mid);
            update(l, r, v, 2*i+1, mid+1, e);
        }
        if (tag[i]) cover[i] = length[i];
        else cover[i] = (i<n ? cover[2*i]+cover[2*i+1] : 0);
    }
};
//*/

////////////////////////////////////////////////////////////////////////
// t mod K Rectangle Union -- O(knlog(n)) Uses the SegmentTree
// Finds the area of the union of axis-aligned rectangles
// Allows storing of prefix areas up to specific points, for prefix sums
// USAGE:
//  1. add_rect(x1,y1,x2,y2); with x1<x2 and y1<y2
//   This defines the rectangle [x1,x2] x [y1,y2]
//  2. add_query(l,r); with l<r. Query is for the area with l<x<r
//  3. solve(); to compute the areas, should only be called ONCE
//  4. query(t,l,r); to query t mod K area
//*!
template<int K> struct ModRectangleUnion {
    struct Segment {
        int a, b, v;
        Segment(): v(0) {}
        Segment(int a, int b, int v): a(a), b(b), v(v) {}
    };
    map<ll,vector<Segment>> segs;
    map<ll,int> cmpr;
    vector<ll> decmp;
    unordered_map<ll,ll> area[K];

    void add_rect(ll a, ll b, ll x, ll y) {
        segs[a].push_back(Segment(b,y,1));
        segs[x].push_back(Segment(b,y,-1));
        cmpr[b]; cmpr[y];
    }
    void add_query(ll a, ll b) {
        segs[a].push_back(Segment());
        segs[b].push_back(Segment());
    }
    ll query(int t, ll a, ll b) {
        if (!area[t].count(a) || !area[t].count(b)) {
            throw runtime_error("query not added");
        }
        return area[t][b] - area[t][a];
    }

    struct Node {
        ll cover[K], tag, length;
        Node(ll l=0): tag(0), length(l) {
            memset(cover, 0, sizeof cover);
            if (K>1) cover[0] = length;
        }
        bool put(int v) { return true; }
        bool get() { return true; }
        void update(int v, int len) {
            tag += v;
            if (len==1) {
                memset(cover, 0, sizeof cover);
                if (K>1) cover[tag%K] = length;
                else cover[0] = (tag ? length : 0);
            }
        }
        void push(Node& left, Node& right, int len) {}
        Node pull(const Node& left, const Node& right) {
            length = left.length + right.length;
            if (K>1) {
                int s = tag%K;
                for (int i=0; i<K; i++) {
                    cover[(i+s)%K] = left.cover[i] + right.cover[i];
                }
            } else {
                cover[0] = (tag ? length : left.cover[0] + right.cover[0]);
            }
            return *this;
        }
    };

    void solve() {
        int cnt = 0;
        vector<ll> len;
        for (auto it = cmpr.begin(); it != cmpr.end(); it++) {
            if (next(it) == cmpr.end()) len.push_back(0);
            else len.push_back(next(it)->first - it->first);
            decmp.push_back(it->first);
            it->second = cnt++;
        }
        SegmentTree<Node,int> segt(cnt);
        for (int i=0; i<cnt; i++) {
            segt.set(i, len[i]);
        }
        segt.build();

        ll cur[K]; memset(cur, 0, sizeof cur);
        ll pre = 0x5F3759DF;
        for (const pair<int,vector<Segment>>& it : segs) {
            for (int i=0; i<K; i++) {
                cur[i] += segt.segt[1].cover[i] * (it.first - pre);
                pre = it.first;
                for (const Segment& s : it.second) {
                    if (s.v == 0) area[i][it.first] = cur[i];
                    else segt.update(cmpr[s.a], cmpr[s.b]-1, s.v);
                }
            }
        }
    }
};
//*/

////////////////////////////////////////////////////////////////////////
/*
int main() {
    return 0;
}
*/
