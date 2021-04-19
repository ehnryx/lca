/* Suffix Array (Linear Time)
 * source: Two Efficient Algorithms for Linear Time Suffix Array Construction
 *         (Ge Nong, Sen Zhang, Daricks Wai Hong Chan)
 * USAGE
 *  suffix_array sa(string_s)
 * INPUT
 *  string or basic_string<int>
 *  remap values to [1,n] if using basic_string<int>
 * TIME
 *  O(N)
 *  N = |string|
 * STATUS
 *  tested: cf/102803c
 * NOTE
 *  I have no idea how what is going on in this code
 *  #illegible
 */
#pragma once

template <typename T>
struct suffix_array {
  vector<int> sa, inv, height;

  suffix_array(const basic_string<T>& s) {
    int n = size(s) + 1;
    int *t = new int[n], *sf = new int[n];
    int alpha = t[n-1] = 0;
    for(int i=0; i<n-1; i++) alpha = max(alpha, t[i] = int(s[i]));
    build(n, t, sf, alpha);
    copy(sf, sf + n, back_inserter(sa));
    inv.resize(n);
    for(int i=0; i<n; i++) inv[sa[i]] = i;
    height.resize(n);
    for(int i=0, h=0; i<n; i++) if(inv[i] > 0) {
      int j = sa[inv[i] - 1];
      while(i+h < n && j+h < n && t[i+h] == t[j+h]) h++;
      height[inv[i]] = h;
      if(h > 0) h--;
    }
    delete[] sf; delete[] t;
  }

  void build(int n, int* s, int* sf, int A) {
    vector<bool> t(n); t[n-1] = 1;
    for(int i=n-2; i>=0; i--) {
      t[i] = s[i] < s[i+1] || (s[i] == s[i+1] && t[i+1]);
    }
    int *bkt = new int[A+1];
    buckets(n, s, bkt, A, true);
    memset(sf, -1, n * sizeof(int));
    for(int i=1; i<n; i++) {
      if(t[i] && !t[i-1]) sf[--bkt[s[i]]] = i;
    }
    induce(n, s, t, sf, bkt, A);
    delete[] bkt;

    int m = 0;
    for(int i=0; i<n; i++) {
      if(sf[i] > 0 && t[sf[i]] && !t[sf[i] - 1]) sf[m++] = sf[i];
    }
    memset(sf + m, -1, (n - m) * sizeof(int));
    int name = 0, pre = -1;
    for(int i=0; i<m; i++) {
      int pos = sf[i]; bool diff = false;
      for(int d=0; d<n; d++) {
        if(pre == -1 || s[pos+d] != s[pre+d] || t[pos+d] != t[pre+d]) {
          diff = true;
          break;
        }
        if(d > 0 && ((t[pos+d] && !t[pos+d-1]) || (t[pre+d] && !t[pre+d-1]))) {
          break;
        }
      }
      if(diff) {
        name++;
        pre = pos;
      }
      sf[m + pos/2] = name - 1;
    }
    for(int i=n-1, j=n-1; i>=m; i--) {
      if(sf[i] >= 0) sf[j--] = sf[i];
    }

    int *nsf = sf, *ns = sf + n - m;
    if(name < m) {
      build(m, ns, nsf, name - 1);
    } else {
      for(int i=0; i<m; i++) {
        nsf[ns[i]] = i;
      }
    }

    bkt = new int[A+1];
    buckets(n, s, bkt, A, true);
    for(int i=1, j=0; i<n; i++) {
      if(t[i] && !t[i-1]) ns[j++] = i;
    }
    for(int i=0; i<m; i++) nsf[i] = ns[nsf[i]];
    memset(sf + m, -1, (n - m) * sizeof(int));
    for(int i=m-1; i>=0; i--) {
      int j = sf[i];
      sf[i] = -1;
      sf[--bkt[s[j]]] = j;
    }
    induce(n, s, t, sf, bkt, A);
    delete[] bkt;
  }

  void buckets(int n, int* s, int* bkt, int A, bool end) {
    memset(bkt, 0, A * sizeof(int));
    for(int i=0; i<n; i++) bkt[s[i]]++;
    for(int i=0, sum=0; i<=A; i++) {
      sum += bkt[i];
      bkt[i] = end ? sum : sum - bkt[i];
    }
  }

  void induce(int n, int* s, const vector<bool>& t, int* sf, int* bkt, int A) {
    buckets(n, s, bkt, A, false);
    for(int i=0; i<n; i++) {
      int j = sf[i] - 1;
      if(j >= 0 && !t[j]) sf[bkt[s[j]]++] = j;
    }
    buckets(n, s, bkt, A, true);
    for(int i=n-1; i>=0; i--) {
      int j = sf[i] - 1;
      if(j >= 0 && t[j]) sf[--bkt[s[j]]] = j;
    }
  }
};
