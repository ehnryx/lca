/* Strongly Connected Components (Tarjan's Algorithm)
 * USAGE
 *  strongly_connected<remove_dups> scc(graph);
 * INPUT
 *  graph:  adjacency list of an unweighted directed graph
 *  remove_dups: boolean "remove duplicate edges?"
 * MEMBERS
 *  group: the components. group[i] = vector of nodes in the ith component
 *  remap: maps vertex -> index in their component
 *  scc: the component id. scc[u] = id of the component to which u belongs
 *  dag:  the adjacency list for the dag of scc's
 *  indegree: the in-degree of the nodes in the dag
 *  operator[]: alias for scc
 * TIME
 *  O(V + E) for remove_dups = false
 *  O(V + ElogE) for remove_dups = true
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: cf/1515g
 */
#pragma once

#include <vector>
#include <stack>

template <bool build_dag, typename = void>
struct scc_dag_base {
  scc_dag_base(size_t) {}
};

template <bool build_dag>
struct scc_dag_base<build_dag, std::enable_if_t<build_dag>> {
  std::vector<int> remap;
  std::vector<std::vector<int>> group;
  std::vector<int> indegree;
  std::vector<std::vector<int>> dag;
  scc_dag_base(size_t n): remap(n) { group.reserve(n); }
};

template <bool build_dag = false, bool remove_dups = true>
struct strongly_connected : scc_dag_base<build_dag> {
  using base = scc_dag_base<build_dag>;
  int components;
  std::vector<int> idx, low, scc;
  int operator [] (int i) const { return scc[i]; }
  int size() const { return components; }

  strongly_connected(const std::vector<std::vector<int>>& graph):
    base(graph.size()), components(0),
    idx(graph.size(), -1), low(graph.size(), -1), scc(graph.size(), -1) {
    for (int i = 0, j = 0; i < (int)graph.size(); i++) {
      if (low[i] == -1) {
        dfs(graph, i, j);
      }
    }
    if constexpr (build_dag) {
      base::dag.resize(size());
      base::indegree.resize(size());
      for (int i = 0; i < size(); i++) {
        for (int u : base::group[i]) {
          for (int v : graph[u]) {
            if (scc[v] == i) continue;
            base::dag[i].push_back(scc[v]);
          }
        }
        if constexpr (remove_dups) {
          sort(begin(base::dag[i]), end(base::dag[i]));
          base::dag[i].resize(
              unique(begin(base::dag[i]), end(base::dag[i])) - begin(base::dag[i]));
        }
        for (int j : base::dag[i]) {
          base::indegree[j] += 1;
        }
      }
    }
  }

  void dfs(const std::vector<std::vector<int>>& graph, int u, int& i) {
    static std::stack<int> stk;
    stk.push(u);
    idx[u] = low[u] = i++;
    for (int v : graph[u]) {
      if (low[v] == -1) dfs(graph, v, i);
      if (scc[v] == -1) low[u] = std::min(low[u], low[v]);
    }
    if (idx[u] == low[u]) {
      if constexpr (build_dag) base::group.emplace_back();
      for (int v = -1; v != u; ) {
        v = stk.top();
        stk.pop();
        scc[v] = components;
        if constexpr (build_dag) {
          base::remap[v] = (int)base::group.back().size();
          base::group.back().push_back(v);
        }
      }
      components++;
    }
  }
};

