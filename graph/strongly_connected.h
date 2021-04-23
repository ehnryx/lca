/* Strongly Connected Components (Tarjan's Algorithm)
 * USAGE
 *  strongly_connected<remove_dups> scc(graph);
 * INPUT
 *  graph:  adjacency list of an unweighted directed graph
 *  remove_dups: boolean "remove duplicate edges?"
 * MEMBERS
 *  group: the components. group[i] = vector of nodes in the ith component
 *  low: the component id. low[u] = id of the component to which u belongs
 *  dag:  the adjacency list for the dag of scc's
 *  indegree: the in-degree of the nodes in the dag
 *  operator[]: alias for low
 * TIME
 *  O(V + E) for remove_dups = false
 *  O(V + ElogE) for remove_dups = true
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: nadc21/f
 */
#pragma once

template <bool remove_dups = true>
struct strongly_connected {
  vector<int> idx, low, remap, indegree;
  vector<vector<int>> group, dag;
  int operator [] (int i) const { return low[i]; }

  strongly_connected(const vector<vector<int>>& graph) {
    int n = (int)size(graph);
    idx.resize(n, -1);
    low.resize(n, n);
    remap.resize(n);

    int components = 0;
    for (int i = 0, j = 0; i < n; i++) {
      if (low[i] == n) {
        dfs(graph, i, j, components);
      }
    }
    group.resize(components);
    for (int i = 0; i < n; i++) {
      assert(low[i] < n);
      low[i] = remap[low[i]];
      group[low[i]].push_back(i);
    }

    dag.resize(components);
    indegree.resize(components);
    for (int i = 0; i < components; i++) {
      for (int u : group[i]) {
        for (int v : graph[u]) {
          if (low[v] == i) continue;
          dag[i].push_back(low[v]);
        }
      }
      if constexpr (remove_dups) {
        sort(begin(dag[i]), end(dag[i]));
        dag[i].resize(unique(begin(dag[i]), end(dag[i])) - begin(dag[i]));
      }
      for (int j : dag[i]) {
        indegree[j] += 1;
      }
    }
  }

  int dfs(const vector<vector<int>>& graph, int u, int& i, int& r) {
    idx[u] = low[u] = i++;
    for (int v : graph[u]) {
      if(low[v] == (int)size(graph)) {
        low[u] = min(low[u], dfs(graph, v, i, r));
      } else if(idx[v] != -1) {
        low[u] = min(low[u], idx[v]);
      }
    }
    if (idx[u] == low[u]) {
      remap[low[u]] = r++;
    }
    idx[u] = -1;
    return low[u];
  }
};

