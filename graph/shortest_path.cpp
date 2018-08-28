#include <bits/stdc++.h>
using namespace std;

// INF > 1e9, INF+INF does not overflow int
// can memset to INF because repeating bytes
const int INF = 0x3f3f3f3f;

// Edge struct for adjacency lists of weighted graphs
// id: adjacent node
// cost: weight of edge
struct Edge {
    int id, cost;
    Edge() {}
    Edge(int i, int c): id(i), cost(c) {}
    // reverse comparator for priority queue (max heap)
    bool operator < (const Edge& v) const {
        return cost > v.cost;
    }
};

////////////////////////////////////////////////////////////////////////
// Dijkstra -- O(Elog(E)) (UNTESTED)
// Finds the shortest path from s to t on a weighted directed graph
// Add u->v and u<-v for undirected graphs
// weights MUST be non-negative
//*!
int dijkstra(const vector<vector<Edge>>& adj, int s, int t) {
    vector<int> dist(adj.size(), -1);
    priority_queue<Edge> todo;
    todo.push(Edge(s, 0));
    while (!todo.empty()) {
        Edge u = todo.top(); todo.pop();
        if (dist[u.id] == -1) {
            dist[u.id] = u.cost;
            // we could also check whether u.id == t to break early
            for (const Edge& e : adj[u.id]) {
                if (dist[e.id] == -1) {
                    todo.push(Edge(e.id, u.cost + e.cost));
                }
            }
        }
    }
    return dist[t];
}
//*/

////////////////////////////////////////////////////////////////////////
// Bellman-Ford -- O(VE) (UNTESTED)
// Finds the shortest path from s to t on a weighted directed graph
// Add u->v and u<-v for undirected graphs
// NO restriction on weights
//*!
int bellmanford(const vector<vector<Edge>>& adj, int s, int t) {
    int n = adj.size();
    vector<int> dist(n, INF);
    dist[s] = 0;
    for (int r = 1; r < n; r++) {
        for (int u = 0; u < n; u++) {
            for (const Edge& e : adj[u]) {
                dist[e.id] = min(dist[e.id], dist[u] + e.cost);
            }
        }
    }
    return dist[t];
}
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
