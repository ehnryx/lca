/* Strongly Connected Components (Tarjan's Algorithm)
 * USAGE
 *  strongly_connected<remove_dups> scc(graph);
 * INPUT
 *  graph:  adjacency list of an unweighted directed graph
 *  remove_dups: boolean "remove duplicate edges?"
 * MEMBERS
 *  group: the components. group[i] = vector of nodes in the ith component
 *  remap: maps vertex -> index in their component
 *  scc: the component id. low[u] = id of the component to which u belongs
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

template <bool remove_dups = true>
struct strongly_connected {
  vector<int> idx, low, scc, remap;
  vector<vector<int>> group;
#ifdef USE_SCC_DAG
  vector<int> indegree;
  vector<vector<int>> dag;
#endif
  int operator [] (int i) const { return scc[i]; }
  int size() const { return (int)group.size(); }

  strongly_connected(const vector<vector<int>>& graph):
    idx(graph.size(), -1), low(graph.size(), -1),
    scc(graph.size()), remap(graph.size()) {
    group.reserve(graph.size());
    for (int i = 0, j = 0; i < (int)graph.size(); i++) {
      if (low[i] == -1) {
        dfs(graph, i, j);
      }
    }
#ifdef USE_SCC_DAG
    dag.resize(size());
    indegree.resize(size());
    for (int i = 0; i < size(); i++) {
      for (int u : group[i]) {
        for (int v : graph[u]) {
          if (scc[v] == i) continue;
          dag[i].push_back(scc[v]);
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
#endif
  }

  int dfs(const vector<vector<int>>& graph, int u, int& i) {
    static stack<int> stk;
    stk.push(u);
    idx[u] = low[u] = i++;
    for (int v : graph[u]) {
      if(low[v] == -1) {
        low[u] = min(low[u], dfs(graph, v, i));
      } else if(idx[v] != -1) {
        low[u] = min(low[u], idx[v]);
      }
    }
    if (idx[u] == low[u]) {
      vector<int> cur_group;
      for (int v = -1; v != u; ) {
        v = stk.top();
        stk.pop();
        idx[v] = -1;
        scc[v] = (int)size();
        remap[v] = (int)cur_group.size();
        cur_group.push_back(v);
      }
      group.push_back(move(cur_group));
    }
    return low[u];
  }
};

