#pragma GCC optimize("O3")
#pragma GCC optimize("fast-math")

#include <bits/stdc++.h>
using namespace std;

#ifdef HENRYX
#include "../delaunay.h"
#else
#include "delaunay.h"
#endif

constexpr char nl = '\n';
//using ld = long double;
using ld = double;
// eps = 1e-9 passed for ld = long double, but failed for ld = double
// eps = 1e-8 passed with 5/7 probability (with ld = double)
// eps = 2e-8 passed with 7/8 probability (with ld = double)

// to replace
namespace ubc {
  using pt = complex<ld>;
  using pol = vector<pt>;
  const ld EPS = 1e-9;

  ld cp(const pt& a, const pt& b) { return imag(conj(a)*b); }
  ld dp(const pt& a, const pt& b) { return real(conj(a)*b); }
  int sgn(const ld& x) { return x < -EPS ? -1 : EPS < x ? 1 : 0; }
  bool cmp_lex(const pt& a, const pt& b) {
    return a.real()<b.real()-EPS
      ||  (a.real()<=b.real()+EPS && a.imag()<b.imag()-EPS); }

  // handles ALL cases, uncomment the 3 marked lines to exclude endpoints
  bool seg_x_seg(pt a, pt b, pt c, pt d) {
    //if(eq(a, b) || eq(c, d)) return 0; // exclude endpoints
    ld sa=abs(b-a), sc=abs(d-c); sa=sa>EPS?1/sa:0; sc=sc>EPS?1/sc:0;
    int r1 = sgn(cp(b-a, c-a) * sa), r2 = sgn(cp(b-a, d-a) * sa);
    int r3 = sgn(cp(d-c, a-c) * sc), r4 = sgn(cp(d-c, b-c) * sc);
    if(!r1 && !r2 && !r3) { // collinear
      return false; // sad :'(
      if(cmp_lex(b, a)) swap(a, b);
      if(cmp_lex(d, c)) swap(c, d);
      //return cmp_lex(a, d) && cmp_lex(c, b); // exclude endpoints
      return !cmp_lex(d, a) && !cmp_lex(b, c);
    } return r1*r2 <= 0 && r3*r4 <= 0; }//change to < to exclude endpoints

  // Intersection of non-parallel lines a -> b, c -> d.
  pt line_inter(const pt &a, const pt &b, const pt &c, const pt &d) {
    return a + cp(c - a, d - c) / cp(b - a, d - c) * (b - a); }

  // Does NOT include points on the ends of the segment.
  inline bool on_segment(const pt &a, const pt &b, const pt &p) {
    return abs(cp(b-a, p-a)) / abs(a-b) < EPS &&
      dp(b-a, p-a)>0 && dp(a-b, p-b)>0; }

  // Checks if p lies on the boundary of a polygon v.
  inline bool on_boundary(const pol &v, const pt &p) {
    bool res = 0; for(int i=v.size()-1, j=0; j<v.size(); i=j++) {
      res |= on_segment(v[i], v[j], p) || abs(p-v[i]) < EPS; }
    return res; }

  // orientation does not matter !!!
  bool pt_in_polygon(const pt &p, const pol &v, bool strict=false) {
    if (on_boundary(v,p)) return !strict;
    ld res = 0; for(int i = v.size() - 1, j = 0; j < v.size(); i = j++) {
      res += atan2(cp(v[i] - p, v[j] - p), dp(v[i] - p, v[j] - p)); }
    return abs(res) > 1; } // will be either 2*PI or 0
}

int main() {
  cin.tie(0)->sync_with_stdio(0);

  int n;
  cin >> n;
  vector<point<ld>> v;
  vector<complex<ld>> cv;
  for (int i = 0; i < n; i++) {
    int x, y;
    cin >> x >> y;
    v.emplace_back(x, y);
    cv.emplace_back(x, y);
  }
  auto triangles = delaunay(v, 2e-8);
  assert(clock() < CLOCKS_PER_SEC);

  ld ans = 0;
  for (auto [ci, cj, ck] : triangles) {
    auto center = circumcenter(v[ci], v[cj], v[ck]);
    complex<ld> cc(center.x, center.y);
    if (ubc::pt_in_polygon(cc, cv)) {
      ans = max(ans, circumradius(v[ci], v[cj], v[ck]));
      continue;
    }
    for (auto end : {(v[ci]+v[cj])/2, (v[cj]+v[ck])/2, (v[ck]+v[ci])/2}) {
      for (int i = n-1, j = 0; j < n; i = j++) {
        if (!ubc::seg_x_seg(cv[i], cv[j], cc, ubc::pt(end.x, end.y))) continue;
        auto it = ubc::line_inter(cv[i], cv[j], cc, ubc::pt(end.x, end.y));
        ld cur = 1e42;
        for (int k = 0; k < n; k++) {
          cur = min(cur, abs(it - cv[k]));
        }
        ans = max(ans, cur);
      }
    }
  }
  cout << fixed << setprecision(9) << ans << nl;

  return 0;
}
