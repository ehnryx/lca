/* Flow with demands (Circulation)
 * USAGE
 *  flow_with_demands<flow_class> graph(n);
 *  graph.add_edge(a, b, demand, capacity);
 *  graph.flowable(source, sink);
 * MEMBERS
 *  void add_edge(a, b, demand, capacity); adds an edge a -> b
 *  bool flowable(source, sink);
 *    adds the edge sink -> source with capacity inf, and calls flowable();
 *  bool flowable(); returns whether a circulation exists.
 *    *** flowable should only be called once!
 * TIME
 *  O(flow)
 * STATUS
 *  tested: cf/100199b
 */
#pragma once

template <template <typename> class Flow, typename T>
struct flow_with_demands : Flow<T> {
  int source, sink;
  vector<T> in, out;
  flow_with_demands(int n): Flow<T>(n + 2), source(n), sink(n + 1), in(n), out(n) {}

  void add_edge(int a, int b, const T& d, const T& c) {
    assert(a < source && b < source && d <= c);
    if (c != d) {
      Flow<T>::add_edge(a, b, c - d);
    }
    in[b] += d;
    out[a] += d;
  }

  bool flowable(int s, int t) {
    assert(s < source && t < source);
    Flow<T>::add_edge(t, s, numeric_limits<T>::max());
    return flowable();
  }

  bool flowable() {
    T saturated = 0;
    for (int i = 0; i < source; i++) {
      if (in[i] != 0) {
        Flow<T>::add_edge(source, i, in[i]);
        saturated += in[i];
      }
      if (out[i] != 0) {
        Flow<T>::add_edge(i, sink, out[i]);
      }
    }
    return Flow<T>::flow(source, sink) == saturated;
  }
};

