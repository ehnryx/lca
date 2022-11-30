/* Flow with demands (Circulation)
 * USAGE
 *  flow_with_demands<flow_class> graph(n);
 *  graph.add_edge(a, b, demand, capacity);
 *  graph.flowable(source, sink);
 * MEMBERS
 *  void add_edge(a, b, demand, capacity); adds an edge a -> b
 *  bool flowable(source, sink);
 *    adds the edge sink -> source with capacity inf, and calls flowable();
 *  bool flow(source, sink); returns whether a circulation exists.
 *  void finalize(); completes the flow with demands graph.
 *    Should be called before flowing
 * TIME
 *  O(flow)
 * STATUS
 *  tested: cf/100199b
 */
#pragma once

#include <numeric>
#include <stdexcept>
#include <vector>

template <template <typename> class Flow, typename T>
struct flow_with_demands : Flow<T> {
  int source, sink;
  std::vector<T> in, out;
  T saturated;
  flow_with_demands(int n):
    Flow<T>(n + 2), source(n), sink(n + 1), in(n), out(n), saturated(0) {}

  void add_edge(int a, int b, const T& d, const T& c) {
    if (a >= source or b >= source) {
     throw std::invalid_argument("add_edge: node indices out of bounds");
    }
    if (d > c) {
      throw std::invalid_argument("add_edge: demand is larger than capacity");
    }
    if (c != d) {
      Flow<T>::add_edge(a, b, c - d);
    }
    in[b] += d;
    out[a] += d;
  }

  void finalize() {
    for (int i = 0; i < source; i++) {
      if (in[i] != 0) {
        Flow<T>::add_edge(source, i, in[i]);
        saturated += in[i];
      }
      if (out[i] != 0) {
        Flow<T>::add_edge(i, sink, out[i]);
      }
    }
  }

  T flowable(int s, int t, T max_flow = std::numeric_limits<T>::max()) {
    if (s >= source or t >= source) {
     throw std::invalid_argument("flowable: node indices out of bounds");
    }
    Flow<T>::add_edge(t, s, max_flow);
    T res = (Flow<T>::flow(source, sink) == saturated ? Flow<T>::adj[t].back().flow : -1);
    Flow<T>::adj[s].pop_back();
    Flow<T>::adj[t].pop_back();
    return res;
  }

  T min_flowable(int s, int t) {
    T res = flowable(s, t);
    if (res != -1) {
      res -= Flow<T>::flow(t, s);
    }
    return res;
  }
};

