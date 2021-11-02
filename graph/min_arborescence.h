/* Minimum weight (out) arborescence
 * https://www.cs.tau.ac.il/~zwick/grad-algo-13/directed-mst.pdf
 * USAGE
 *  min_arborescence<weight_t, Compare = less<>, sparse = true> arb(n);
 *  arb.add_edge(a, b, weight); // a -> b with weight
 *  arb.build();
 *  arb.solve(root);
 * TIME
 *  O(V + ElogE) for sparse
 *  O(V^2)       for dense
 *  V = #vertices, E = #edges
 * STATUS
 *  untested
 */
#pragma once

#include "../data_structure/leftist_tree.h"
#include "../data_structure/union_find.h"

template <typename T, class Compare = less<>, bool sparse = true>
struct min_arborescence {
  struct Edge {
    int from, to;
    T cost, orig;
    auto operator <=> (const Edge& o) const {
      return cost <=> o.cost;
    }
    void operator += (T v) {
      cost += v;
    }
    bool invalid() const {
      return from == to;
    }
  };

  union_find<true, false> weak;
  union_find<false, false> strong;  // must not swap root
  vector<Edge> enter;
  vector<vector<int>> children;
  vector<int> parent, rep;

  struct dense_data_t : vector<vector<Edge>> {
    dense_data_t(int n): vector<vector<Edge>>(n, vector(n, Edge(-1, -1, 0, 0))) {}
    // store reverse edges for cache goodness
  };

  using heap_t = conditional_t<sparse,
    vector<leftist_tree<Edge, leftist_node_lazy<Edge, T>, Compare>>,
    dense_data_t>;
  heap_t adj;

  min_arborescence(int n):
    weak(n), strong(n), enter(2 * n, Edge(-1, -1, 0, 0)),
    children(2 * n), parent(2 * n, -1), rep(n), adj(n) {}

  void add_edge(int a, int b, T cost) {
    if (a == b) [[unlikely]] return;  // ignore self-loops
    if constexpr (sparse) {
      adj[b].push(Edge(a, b, cost, cost));
    } else {  // dense
      if (adj[b][a].invalid() || Compare()(cost, adj[b][a].cost)) {
        adj[b][a] = Edge(a, b, cost, cost);
      }
    }
  }

  Edge get_edge(int to) {
    if constexpr (sparse) {
      if (adj[to].empty()) {
        return Edge(-1, -1, 0, 0);
      } else {
        Edge e = adj[to].top();
        adj[to].pop();
        return e;
      }
    } else {  // dense
      Edge res(-1, -1, 0, 0);
      for (int i = 0; i < (int)adj.size(); i++) {
        if (!adj[to][i].invalid() && strong.find(i) != to &&
            (res.invalid() || Compare()(adj[to][i], res))) {
          res = adj[to][i];
        }
      }
      return res;
    }
  }

  void lazy_add(int to, T val) {
    if constexpr (sparse) {
      adj[to].lazy_add(val);
    } else {  // dense
      for (int i = 0; i < (int)adj.size(); i++) {
        adj[to][i] += val;  // maybe offset instead of adding immediately ?
      }
    }
  }

  void merge(int from, int to) {
    if constexpr (sparse) {
      adj[to].merge(adj[from]);
    } else {  // dense
      // replace all `from` with `to`
      for (int i = 0; i < (int)adj.size(); i++) {
        if (!adj[from][i].invalid() &&
            (adj[to][i].invalid() || Compare()(adj[from][i], adj[to][i]))) {
          adj[to][i] = adj[from][i];
        }
      }
    }
  }

  void build() {
    vector<int> roots(adj.size());
    iota(rep.begin(), rep.end(), 0);
    iota(roots.begin(), roots.end(), 0);

    int rid = 0;
    int uid = (int)adj.size();
    int scc_num = (int)adj.size();
    while (rid < (int)adj.size() && scc_num > 1) {
      int r = roots[rid++];
      auto [s, t, cost, orig] = get_edge(r);
      if (s == t) continue;  // no incoming edges
      if constexpr (sparse) {
        if (strong.find(s) == r) {
          roots[--rid] = r;
          continue;  // same scc, skip edge
        }
      }
      enter[rep[r]] = Edge(s, t, cost, orig);
      if (weak.find(s) != weak.find(t)) {
        weak.link(s, t);
        continue;  // we don't grow any scc's
      }

      // time to grow the scc
      Edge min_edge = Edge(s, t, cost, orig);
      for (int to = strong.find(s); to != r; ) {
        const Edge& e = enter[rep[to]];
        if (Compare()(e, min_edge)) {
          min_edge = e;
        }
        to = strong.find(e.from);
      }
      lazy_add(r, min_edge.cost - cost);
      for (int to = strong.find(s); to != r; ) {
        const Edge& e = enter[rep[to]];
        lazy_add(to, min_edge.cost - e.cost);
        parent[rep[to]] = uid;
        children[uid].push_back(rep[to]);
        strong.link(to, r);
        merge(to, r);
        scc_num -= 1;
        to = strong.find(e.from);
      }
      parent[rep[r]] = uid;
      children[uid].push_back(rep[r]);
      rep[r] = uid++;
      roots[--rid] = r;
    }
  }

  pair<vector<pair<int, T>>, T> solve(int root) {
    T sum = 0;
    vector<pair<int, T>> in(adj.size(), pair(-1, 0));
    int scc_root = strong.find(root);
    sum += dismantle(rep[scc_root], root, in);
    for (int i = 0; i < (int)adj.size(); i++) {
      if (i == strong.find(i) && i != scc_root) {
        const Edge& e = enter[rep[i]];
        if (e.invalid()) continue;
        in[e.to] = pair(e.from, e.orig);
        sum += e.orig + dismantle(rep[i], e.to, in);
      }
    }
    return pair(move(in), sum);
  }

  T dismantle(int scc, int root, vector<pair<int, T>>& in) {
    T sum = 0;
    while (root != scc) {
      for (int v : children[parent[root]]) {
        if (v == root) continue;
        const Edge& e = enter[v];
        in[e.to] = pair(e.from, e.orig);
        sum += e.orig + dismantle(v, e.to, in);
      }
      root = parent[root];
    }
    return sum;
  }
};

