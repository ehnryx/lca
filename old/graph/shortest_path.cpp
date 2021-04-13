#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// INF > 1e9, INF+INF does not overflow int
// can memset to INF because repeating bytes
// INFLL > 1e18, INF+INF does not overflow ll
const int INF = 0x3f3f3f3f;
const ll INFLL = 0x3f3f3f3f3f3f3f3f;

// Edge struct for adjacency lists of weighted graphs
// id: adjacent node
// cost: weight of edge
// USAGE:
//  1. vector<vector<Edge>> adj(n); or vector<Edge> adj[n];
//     to create an adjacency list. Must have n >= number of nodes.
//  2. adj[a].push_back(Edge(b,c)); 
//     to add the directed edge a->b with cost c
struct Edge {
    int pre, id; ll cost;
    Edge() {}
    Edge(int i, ll c): id(i), cost(c) {}
    Edge(int p, int i, ll c): pre(p), id(i), cost(c) {}
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
// USAGE:
//  1. adj stores the graph as an adjacency list
//  2. min dist from start to dest = dijkstra(adj, start, dest);
//*!
ll dijkstra(const vector<vector<Edge>>& adj, int s, int t) {
    vector<ll> dist(adj.size(), -1);
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
// Bellman-Ford -- O(VE)
// TESTED ON: cf/101498/l, cf/101873/e
// Finds the shortest path from s to t on a weighted directed graph
// Add u->v and u<-v for undirected graphs
// NO restriction on weights
// USAGE:
//  1. edges stores all the edges in the graph
//  2. cycle = bellmanford(edges, numNodes, start, dest, ans);
//  3. ans stores the minimum distance from start to dest
//  4. cycle is a bool that is True iff there is a negative cycle
//*!
bool bellmanford(const vector<Edge>& edges, int n, int s, int t, ll& res) {
    vector<ll> dist(n, INFLL);
    dist[s] = 0;
    for (int r = 1; r < n; r++) {
        for (const Edge& e : edges) {
            dist[e.id] = min(dist[e.id], dist[e.pre] + e.cost);
        }
    }
    res = dist[t];
    // find negative cycles
    for (const Edge& e : edges) {
        if (dist[e.pre] + e.cost < dist[e.id]) {
            return true;
        }
    }
    return false;
}
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
