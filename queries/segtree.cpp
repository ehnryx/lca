#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

////////////////////////////////////////////////////////////////////////
// Templated Segment Tree -- O(log(n)) per reasonable query
// Warning: possibly SLOW! because Node is copied when querying
//
// USAGE:
// 	SegmentTree<Node> magic(length);
// 	magic.set(i,v); sets ith value to v, DOES NOT PULL
// 	magic.build(); pulls leaves up
// 	magic.query(l,r); range [l,r], combines segments using pull
// 	magic.update(l,r,v); range [l,r], updates with v
// 
// The Node class requires the following: (SEE EXAMPLE BELOW)
// Node();
//  The default constructor that is called initially, and when a query
//  range is out of bounds.
// Node(T v);
//  The constructor that is called in set(int i, T v);
// bool put();
//  The break condition when updating. For normal queries, return true.
// bool get();
//  The break condition when querying. For normal queries, return true.
// void update(T v, int len);
//  Used to update the Node with value v at a segment with length len.
// void push(Node& left, Node& right, int len);
//  Used to lazy propagate from current node with length len to left 
//  and right children.
// Node pull(Node left, Node right);
//  Used to update current node from left and right children.
//*!
template <class Node, class T>
struct SegmentTree {
    int n;
    vector<Node> segt;
    SegmentTree(int len) {
        n = 1 << (32 - __builtin_clz(len-1));
        segt.resize(2*n);
    }
    void set(int i, const T& v) {
        segt[n+i] = Node(v);
    }
    void build() {
        for (int i = n-1; i > 0; i--) {
            segt[i].pull(segt[2*i], segt[2*i+1]);
        }
    }
    void update(int l, int r, const T& v) {
        update(l, r, v, 1, 0, n-1);
    }
    void update(int l, int r, const T& v, int i, int s, int e) {
        if (e < l || s > r) return;
        if (l <= s && e <= r && segt[i].put()) {
            segt[i].update(v, e-s+1);
            return;
        }
        segt[i].push(segt[2*i], segt[2*i+1], e-s+1);
        int m = (s + e) / 2;
        update(l, r, v, 2*i, s, m);
        update(l, r, v, 2*i+1, m+1, e);
        segt[i].pull(segt[2*i], segt[2*i+1]);
    }
    Node query(int l, int r) {
        return query(l, r, 1, 0, n-1);
    }
    Node query(int l, int r, int i, int s, int e) {
        if (e < l || s > r) return Node();
        if (l <= s && e <= r && segt[i].get()) {
            return segt[i];
        }
        segt[i].push(segt[2*i], segt[2*i+1], e-s+1);
        int m = (s + e) / 2;
        return Node().pull(query(l, r, 2*i, s, m), query(l, r, 2*i+1, m+1, e));
    }
};
//*/

////////////////////////////////////////////////////////////////////////
// Example with range minimum query and range assignment update

struct Int {
    int x, d;
    bool lazy;
    Int(int x=0x3f3f3f3f): x(x), d(x), lazy(true) {}
    bool get() { return true; }
    bool put() { return true; }
    void update(int v, int len) {
        x = d = v;
        lazy = true;
    }
    void push(Int& left, Int& right, int len) {
        if (lazy) {
            left.x = right.x = d;
            left.d = right.d = d;
            left.lazy = right.lazy = true;
            lazy = false;
        }
    }
    const Int& pull(const Int& left, const Int& right) {
        x = min(left.x, right.x);
        lazy = false;
        return *this;
    }
};

int main() {
    SegmentTree<Int,int> rmq(6);
    rmq.set(0, 4);
    rmq.set(1, 6);
    rmq.set(2, 3);
    rmq.set(3, 5);
    rmq.set(4, 7);
    rmq.set(5, 1);
    rmq.build();
    cout << rmq.query(0,2).x << endl; // 3
    cout << rmq.query(3,5).x << endl; // 1
    rmq.update(1,3,2);
    cout << rmq.query(0,1).x << endl; // 2
    cout << rmq.query(4,4).x << endl; // 7
    cout << rmq.query(2,5).x << endl; // 1
    return 0;
}
