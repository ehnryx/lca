/**
 * Author: Henry X
 * Date: 2020 April
 * Source: https://sites.math.rutgers.edu/~sk1233/courses/ANT-F14/lec2.pdf
 * Description: approximate x in [0,1) with rational a/b with b<=N
 * Time: O(poly(\log N))
 * Status: tested on kattis/bestrational, gcj2019/round2
 */

// approximate x in [0,1) with rational a/b with b<=N
// https://sites.math.rutgers.edu/~sk1233/courses/ANT-F14/lec2.pdf
pair<ll, ll> approximate(ld x, ll N) {
	ll op = 1, oq = 0, p = 0, q = 1, a = 0;
	for(ld y = 1/x; ; y = 1/(y-a)) {
		if(abs(y) > 3*N) return make_pair(p, q);
		a = floor(y);
		if(q > (N - oq) / a) {
			ll t = (N - oq) / q;
			ll tp = t*p + op, tq = t*q + oq;
			ld A = (ld)p/q, B = (ld)tp/tq;
			return abs(A-x) < abs(B-x) ? make_pair(p, q) : make_pair(tp, tq);
		}
		op += p*a; swap(op, p);
		oq += q*a; swap(oq, q);
	}
}
