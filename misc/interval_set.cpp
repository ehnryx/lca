#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int,int> pii;

const int INF = 0x3f3f3f3f;

////////////////////////////////////////////////////////////////////////
// Interval Set -- amortized O(log(n)) per operation
// TESTED with excl/excl on USP tryouts 2018
// Stores a set of disjoint intervals,
// interval is templated, but default is [a,b)
// insert inserts the interval
// find returns the interval or {inf,inf} if not found
// USAGE:
//  1. Define a type with operators < and ==, Also an inf for that type
//  2. IntervalSet<Type,left_inclusive,right_inclusive> iset(T.inf);
//  3. iset.insert(a,b); to insert the interval (a,b)
//  4. iset.find(x); to find to which interval point x belongs
//*!
template <class T, bool lin=true, bool rin=false>
struct IntervalSet {
    T inf;
    set<pair<T,T>> s;
    IntervalSet(T inf): inf(inf) {}
    bool cmp(T a, T b, bool l, bool r=false) {
        if (l||r) return a < b || a == b;
        else return a < b;
    }
    void insert(T a, T b) {
        if (s.empty()) { s.insert({a,b}); return; }
        auto it = s.upper_bound({a,inf});
        while (it != s.end() && cmp(it->first,b,lin,rin)) {
            b = max(b, it->second);
            it = s.erase(it);
        }
		it = s.upper_bound({a,inf});
		while (it != s.begin() && cmp(a,(--it)->second,lin,rin)) {
			a = min(a, it->first);
			b = max(b, it->second);
			it = s.erase(it);
		}
		s.insert({a,b});
    }
    pair<T,T> find(T x) {
        if (s.empty()) { return {inf,inf}; }
        auto it = s.upper_bound({x,inf});
        if (it == s.begin()) return {inf,inf};
        if (cmp((--it)->second,x,!rin)) return {inf,inf};
        if (cmp(x,it->first,!lin)) return {inf,inf};
        return *it;
    }
};
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
