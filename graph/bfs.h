/* Breadth First Search
 * USAGE
 *  vector<int> dist = bfs(graph, source);
 * INPUT
 *  graph:  adjacency list of an unweighted directed graph
 *  source: the vertex from which to run BFS
 * OUTPUT
 *  dist[i] = distance (number of edges) from source to i
 *  dist[i] = -1 if it is impossible to reach i from source
 * TIME
 *  O(V + E)
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: kattis/shortestpath3
 */
#pragma once

vector<int> bfs(
    const vector<vector<int>>& graph,
    int source) {
  vector<int> dist(size(graph), -1);
  queue<int> todo;
  todo.push(source);
  dist[source] = 0;
  while (!todo.empty()) {
    // get u from the queue
    int u = todo.front();
    todo.pop();
    for (int v : graph[u]) {
      // follow the edge u -> v if v is unvisited
      if (dist[v] == -1) {
        dist[v] = dist[u] + 1;
        todo.push(v);
      }
    }
  }
  return dist;
}

