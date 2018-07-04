#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

const ll MOD = 1e9+7;

////////////////////////////////////////////////////////////////////////
// Extended Euclidean Algorithm (UBC)
// egcd(a,b,x,y) calculates x,y such that ax+by=gcd(a,b)
// To find a^{-1} mod m, use egcd(a,m,x,y), then x = a^{-1}
// Returns gcd(a,b)
//*!
ll egcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    } else {
        ll d = egcd(b, a%b, y, x);
        y -= x * (a/b);
        return d;
    }
}
//*/

////////////////////////////////////////////////////////////////////////
// Int struct for operations on a prime field (TESTED)
// WARNING: slow
//*!
struct Int {
    ll x;
    Int (ll n=0) { x = n % MOD; if (x < 0) x += MOD; }
    operator ll() { return x; }
    Int operator + (const Int& n) const { return Int(x + n.x); }
    Int operator - (const Int& n) const { return Int(x - n.x); }
    Int operator * (const Int& n) const { return Int(x * n.x); }
    Int operator / (const Int& n) const { return Int(x * n.inv()); }
    void operator += (const Int& n) { x = (x + n.x) % MOD; }
    void operator -= (const Int& n) { x = (x + MOD - n.x) % MOD; }
    void operator *= (const Int& n) { x = (x * n.x) % MOD; }
    void operator /= (const Int& n) { x = (x * n.inv()) % MOD; }
    ll inv() const {
        if (x == 0) throw runtime_error("divide by zero");
        ll c, d;
        egcd(x, MOD, c, d);
        return (c < 0) ? c+MOD : c;
    }
};
//*/

////////////////////////////////////////////////////////////////////////
// Rational struct (UNTESTED)
//*!
struct Frac {
    ll n, d;
    Frac(ll a = 0, ll b = 1) {
        if (b == 0) {
            n = (a==0 ? 0 : a/abs(a));
            d = 0;
        } else {
            ll g = __gcd(a,b); 
            n = a/g; d = b/g;
            if (d < 0) { n = -n; d = -d; }
        }
    }
    friend ostream& operator << (ostream& os, const Frac& f) {
        return os << '(' << f.n << '/' << f.d << ')';
    }
    operator ld() { return (ld)n/d; }
    Frac operator + (const Frac& f) const { return Frac(n*f.d + d*f.n, d*f.d); }
    Frac operator - (const Frac& f) const { return Frac(n*f.d - d*f.n, d*f.d); }
    Frac operator * (const Frac& f) const { return Frac(n*f.n, d*f.d); }
    Frac operator / (const Frac& f) const { return Frac(n*f.d, d*f.n); }
    bool operator == (const Frac& f) const {
        if (!n && !d) return false;
        if (!f.n && !f.d) return false;
        return n*f.d == d*f.n;
    }
    bool operator < (const Frac& f) const {
        if (!n && !d) return false;
        if (!f.n && !f.d) return true;
        return n*f.d < d*f.n;
    }
};
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
