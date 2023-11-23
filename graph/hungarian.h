/* Max weight bipartite matching (Hungarian Algorithm)
 * USAGE
 *  hungarian<T> graph(n, m); // bipartite K_{n,m}
 *  graph.add_weight(a, b, w); // non-negative, default = 0
 *  T weight = graph.run();
 * TIME
 *  O(cubic)
 * STATUS
 *  tested: dmoj/tle17c7p5
 */
#pragma once

#include <cassert>
#include <numeric>
#include <stdexcept>
#include <vector>

template <class T>
struct hungarian {
  int n, m;
  std::vector<std::vector<T>> weight;
  std::vector<int> match;  // match[l] = r
  hungarian(int _n, int _m) : n(_n), m(_m), weight(n, std::vector<T>(m)), match(n, -1) {
    if (n > m) throw std::invalid_argument("expected n <= m");
  }
  void add_weight(int a, int b, const T& w) { weight[a][b] = max(weight[a][b], w); }

  T run() {
    std::vector<int> inv(m, -1);       // inverse of match
    std::vector<T> left(n), right(m);  // labels
    for (int i = 0; i < n; i++) {
      left[i] = *max_element(weight[i].begin(), weight[i].end());
    }

    for (int i = 0; i < n; i++) {
      // adding i into the matching
      std::vector<bool> in_s(n), in_tree(m);
      std::vector<int> parent(m, -1);  // parent in alt-tree
      std::vector<T> slack(m);
      in_s[i] = true;
      for (int j = 0; j < m; j++) {
        slack[j] = left[i] + right[j] - weight[i][j];
      }

      while (true) {
        // find non-slack non-tree vertex
        int add = -1;  // on the right
        for (int j = 0; j < m; j++) {
          if (!in_tree[j] && slack[j] == 0) {
            add = j;
            break;
          }
        }

        // N(S) = T, update labels
        if (add == -1) {
          T sub = std::numeric_limits<T>::max();
          for (int j = 0; j < m; j++) {
            if (slack[j] != 0) {
              sub = min(sub, slack[j]);
            }
          }
          for (int u = 0; u < n; u++) {
            if (in_s[u]) {
              left[u] -= sub;
            }
          }
          for (int j = 0; j < m; j++) {
            if (in_tree[j]) {
              right[j] += sub;
            } else {
              slack[j] -= sub;
            }
          }
          continue;
        }

        // find parent
        for (int u = 0; u < n; u++) {
          if (in_s[u] && left[u] + right[add] - weight[u][add] == 0) {
            parent[add] = u;
            break;
          }
        }

        // already matched, grow tree
        if (inv[add] != -1) {
          in_tree[add] = true;
          int s = inv[add];
          in_s[s] = true;
          for (int j = 0; j < m; j++) {
            slack[j] = min(slack[j], left[s] + right[j] - weight[s][j]);
          }
        }

        // augment
        else {
          while (parent[add] != i) {
            int t = match[parent[add]];
            match[parent[add]] = add;
            inv[add] = parent[add];
            add = t;
          }
          match[i] = add;
          inv[add] = i;
          break;
        }
      }
    }

    T out = 0;
    for (int i = 0; i < n; i++) {
      assert(match[i] != -1);
      out += weight[i][match[i]];
    }
    return out;
  }
};
