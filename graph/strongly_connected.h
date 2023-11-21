/* Strongly Connected Components (Tarjan's Algorithm)
 * USAGE
 *  strongly_connected scc(graph_t);
 * MEMBERS
 *  group: the components. group[i] = vector of nodes in the ith component
 *  scc: the component id. scc[u] = id of the component to which u belongs
 *  dag: the adjacency list for the dag of scc's
 *  indegree: the in-degree of the nodes in the dag
 *  operator[]: alias for scc
 * TIME
 *  O(V + E) for build dag
 *  O(V + ElogE) if dedup
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: cf/1515g
 */
#pragma once

#include "../utility/traits.h"
#include "graph.h"
#include <vector>
#include <stack>

MAKE_TRAITS(scc_traits,
  (BUILD_DAG, DEDUP_EDGES),
  DAG_DEDUP = BUILD_DAG | DEDUP_EDGES,
);

template <scc_traits>
struct scc_dag_base {
  std::vector<std::vector<int>> group;
  std::vector<int> indegree;
  std::vector<std::vector<int>> dag;
  scc_dag_base(size_t n) { group.reserve(n); }
};

template <>
struct scc_dag_base<scc_traits::NONE> {
  scc_dag_base(size_t) {}
};

template <scc_traits traits = scc_traits::NONE>
struct strongly_connected : scc_dag_base<traits> {
  static constexpr bool build_dag = traits.has_any(scc_traits::BUILD_DAG);
  static constexpr bool dedup_edges = traits.has_any(scc_traits::DEDUP_EDGES);

  int components;
  std::vector<int> idx, low, scc;
  int operator[](int i) const { return scc[i]; }
  int size() const { return components; }

  template <typename weight_t>
  strongly_connected(graph_t<weight_t> const& graph):
    scc_dag_base<traits>(graph.size()), components(0),
    idx(graph.size(), -1), low(graph.size(), -1), scc(graph.size(), -1) {
    for (int i = 0, index = 0; i < graph.size(); i++) {
      if (low[i] == -1) {
        index = build(graph, i, index);
      }
    }
    if constexpr (build_dag) {
      this->dag.resize(size());
      this->indegree.resize(size());
      for (int from = 0; from < size(); from++) {
        std::vector<int>& neighbours = this->dag[from];;
        for (int u : this->group[from]) {
          for (auto const& e : graph[u]) {
            if (int to = scc[e.to]; to != from) {
              neighbours.push_back(to);
              this->indegree[to] += 1;
            }
          }
        }
        if constexpr (dedup_edges) {
          sort(begin(neighbours), end(neighbours));
          for (int i = 1, j = 0; i < neighbours.size(); i++) {
            if (neighbours[i] == neighbours[j]) {
              this->indegree[neighbours[i]] -= 1;
            } else {
              neighbours[++j] = neighbours[i];
            }
          }
        }
      }
    }
  }

  template <typename weight_t>
  int build(graph_t<weight_t> const& graph, int u, int index) {
    static std::stack<int> stk;
    stk.push(u);
    idx[u] = low[u] = index++;
    for (int v : graph[u]) {
      if (low[v] == -1) index = build(graph, v, index);
      if (scc[v] == -1) low[u] = std::min(low[u], low[v]);
    }
    if (idx[u] == low[u]) {
      if constexpr (build_dag) this->group.emplace_back();
      for (int v = -1; v != u; ) {
        v = stk.top();
        stk.pop();
        scc[v] = components;
        if constexpr (build_dag) this->group.back().push_back(v);
      }
      components++;
    }
    return index;
  }
};

