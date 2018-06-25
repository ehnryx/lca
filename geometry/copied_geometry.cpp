#include <bits/stdc++.h>
using namespace std;

typedef long double ld;
typedef complex<ld> pt;
const ld EPS = 1e-10;

////////////////////////////////////////////////////////////////////////
// General 2D Geometry (UBC)
//*!
ld cp(const pt& a, const pt& b) { return imag(conj(a)*b); }
ld dp(const pt& a, const pt& b) { return real(conj(a)*b); }
ld sgn(const ld& x) { return abs(x) < EPS ? 0 : (x < 0 ? -1 : 1); }
// comparators
bool eq(const pt& a, const pt& b) { return abs(a-b) < EPS; }
bool cmp(const pt& a, const pt& b) {
    return a.real()<b.real()-EPS || (a.real()<b.real()+EPS && a.imag()<b.imag()-EPS);
}
bool cmp_i(const pt& a, const pt& b) {
    return a.imag()<b.imag()-EPS || (a.imag()<b.imag()+EPS && a.real()<b.real()-EPS);
}
// for hashing, 1e5 is arbitrary
namespace std {
    bool operator == (const pt& a, const pt& b) { return eq(a,b); }
    bool operator < (const pt& a, const pt& b) { return cmp(a,b); }
    template<> struct hash<pt> {
        size_t operator () (const pt& v) const {
            return hash<ld>()(v.real()*1e5) ^ hash<ld>()(v.imag());
        }
    };
}
//*/

//*! Segment-Segment Intersection
// change the 3 marked lines to exclude endpoints
bool seg_x_seg(pt a, pt b, pt c, pt d) {
    //if(eq(a,b) || eq(c,d)) return false; // uncomment to exclude endpoints
    ld norm_a = eq(a,b) ? 0 : 1/abs(a-b);
    ld norm_c = eq(c,d) ? 0 : 1/abs(c-d);
    int s1 = sgn(cp(b-a,c-a)*norm_a); int s2 = sgn(cp(b-a,d-a)*norm_a);
    int s3 = sgn(cp(d-c,a-c)*norm_c); int s4 = sgn(cp(d-c,b-c)*norm_c);
    if (!s1 && !s2 && !s3) {
        if (cmp(b,a)) swap(a,b); if (cmp(d,c)) swap(c,d);
        //return cmp(a,d) && cmp(c,b); // uncomment to exclude endpoints
        return !cmp(d,a) && !cmp(b,c);
    } else return s1*s2 <= 0 && s3*s4 <= 0; // change to < to exclude endpoints
}
//*/

//*! More 2D Geometry (LARGELY UNTESTED (possible copying mistakes))

// Parallel lines, returns whether lines ab and cd are parallel
bool parallel(const pt& a, const pt& b, const pt& c, const pt& d) {
    return abs(cp(a-b, c-d)) / abs(a-b) < EPS;
}

// Line-Line intersection, returns intersection of lines ab and cd
pt line_inter(const pt& a, const pt& b, const pt& c, const pt& d) {
    return a + cp(c-a, d-c) / cp(b-a, d-c) * (b-a);
}

// Projection of vector ap to vector ab, SIGNED
ld proj_dist(const pt& a, const pt& b, const pt& p) {
    return dp(b-a, p-a) / abs(b-a);
}

// Projection of point p to line ab, returns the point
pt projection(const pt& a, const pt& b, const pt& p) {
    return a + dp(b-a, p-a) / norm(b-a) * (b-a);
}

// Distance from point p to vector ab, SIGNED (left is +, right is -)
ld lp_dist(const pt& a, const pt& b, const pt& p) {
    return cp(b-a, p-a) / abs(b-a);
}

// Distance from point p to line segment ab
ld lsp_dist(const pt& a, const pt& b, const pt& p) {
    return dp(b-a, p-a) > 0 && dp(a-b, p-b) > 0 ?
        abs(cp(b-a, p-a)) / abs(b-a) : min(abs(a-p), abs(b-p));
}

// Closest point on line segment ab to p
pt lsp_closest(const pt& a, const pt& b, const pt& p) {
    if (dp(b-a, p-a) > 0 && dp(a-b, p-b) > 0) {
        return line_inter(a, b, p, p + pt(0,1)*(a-b));
    } else return abs(a-p) < abs(b-p) ? a : b;
}

// Area of polygon, SIGNED
ld area(const vector<pt>& v) {
    int n = v.size();
    ld res = cp(v[n-1], v[0]);
    for (int i = 1; i < n; i++) res += cp(v[i-1], v[i]);
    return res/2;
}

// Checks whether point p is on segment ab, does NOT include endpoints
bool on_segment(const pt& a, const pt& b, const pt& p) {
    return abs(cp(b-a, p-a)) < EPS && dp(b-a, p-a) > 0 && dp(a-b, p-b) > 0;
}

// Checks whether point p lies on boundary of polygon v
bool on_boundary(const vector<pt>& v, const pt& p) {
    int n = v.size();
    for (int i = n-1, j = 0; j < n; i = j++) {
        if (on_segment(v[i], v[j], p) || abs(v[i]-p) < EPS) return true;
    }
    return false;
}

// Checks whether point p lines in polygon v, boundary included
bool pt_in_polygon(const vector<pt>& v, const pt& p) {
    if (on_boundary(v,p)) return true;
    int n = v.size();
    ld res = 0;
    for (int i = n-1, j = 0; j < n; i = j++) {
        res += atan2(cp(v[i]-p, v[j]-p), dp(v[i]-p, v[j]-p));
    }
    return abs(res) > 1;
}

//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
