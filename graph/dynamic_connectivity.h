/* Dynamic Connectivity
 * USAGE
 *  ???
 * TIME
 *  O(log^2(N)) per operation amortized (?)
 *  N = |graph|
 * STATUS
 *  untested untested UNTESTED ????????
 */
#pragma once

#include "../tree/euler_tour_tree.h"

struct dynamic_connectivity_node_base {
  splay_tree<splay_node<int, void>> edges, tree_edges;
  int edge_incidents, tree_incidents;
  dynamic_connectivity_node_base() : edges(), edge_incidents(0), tree_incidents(0) {}
  void add_edge(int v) {
    if (v & 1) {
      tree_edges.insert(v);
      tree_incidents++;
    } else {
      edges.insert(v);
      edge_incidents++;
    }
  }
  void del_edge(splay_node<int, void>* it) {
    if (it->key & 1) {
      tree_incidents--;
      tree_edges.erase(it);
    } else {
      edge_incidents--;
      edges.erase(it);
    }
  }
  void del_edge_to(int v) { del_edge(get_edge(v)); }
  splay_node<int, void>* get_edge(int v) {
    auto e = edges.lower_bound(v << 1);
    if (e == e->nil || (e->key >> 1) != v) {
      e = tree_edges.lower_bound(v << 1);
    }
    return (e == e->nil || (e->key >> 1) != v) ? e->nil : e;
  }
};

template <typename derived_t, typename value_t>
struct dynamic_connectivity_node : euler_tour_node<derived_t, value_t>,
                                   dynamic_connectivity_node_base {
  using base = euler_tour_node<derived_t, value_t>;
  using dc_base = dynamic_connectivity_node_base;
  dynamic_connectivity_node() : base(), dc_base() {}
  dynamic_connectivity_node(const value_t& v) : base(v), dc_base() {}
  void pull() {
    base::pull();
    dc_base::edge_incidents =
        dc_base::edges.size() + base::left->edge_incidents + base::right->edge_incidents;
    dc_base::tree_incidents =
        dc_base::tree_edges.size() + base::left->tree_incidents + base::right->tree_incidents;
  }
};

struct dynamic_connectivity_node_simple final
    : euler_tour_node<dynamic_connectivity_node_simple, void>,
      dynamic_connectivity_node_base {
  using base = euler_tour_node<dynamic_connectivity_node_simple, void>;
  using dc_base = dynamic_connectivity_node_base;
  dynamic_connectivity_node_simple() : base(), dc_base() {}
  void pull() {
    base::pull();
    dc_base::edge_incidents =
        dc_base::edges.size() + base::left->edge_incidents + base::right->edge_incidents;
    dc_base::tree_incidents =
        dc_base::tree_edges.size() + base::left->tree_incidents + base::right->tree_incidents;
  }
};

template <class node_t, bool use_memory_pool>
struct dynamic_forest_layer : euler_tour_tree<node_t, use_memory_pool> {
  using base = euler_tour_tree<node_t, use_memory_pool>;
  using base::nil, base::splay, base::size;
  dynamic_forest_layer(int n) : base(n) {}

  void add_edge(int u, int v, bool separated) {
    splay(v)->add_edge(u << 1 | (int)separated);
    splay(u)->add_edge(v << 1 | (int)separated);
  }

  // [reconnect?][didcut?]
  int try_cut(int u, int v) {
    auto e = splay(u)->get_edge(v);
    if (e == e->nil) return 0b00;
    bool in_tree = e->key & 1;
    splay(u)->del_edge(e);
    splay(v)->del_edge_to(u);
    if (in_tree) {
      base::cut(u, v);
      return 0b11;
    }
    return 0b01;
  }

  template <typename T, bool B>
  std::pair<int, int> reconnect(int u, int v, dynamic_forest_layer<T, B>* forest) {
    if (size(u) > size(v)) std::swap(u, v);
    splay(u);  // for the iteration
    // push all tree edges
    for (auto it = base::find_first_after(nil, _has_tree_incidents, _has_tree_incidents);
         it != nil; it = base::find_first_after(it, _has_tree_incidents, _has_tree_incidents)) {
      int cur = it - &(base::data[0]);
      for (int neighbour : it->tree_edges) {
        neighbour >>= 1;
        splay(neighbour)->del_edge_to(cur);
        forest->link(cur, neighbour);
        forest->add_edge(cur, neighbour, true);
      }
      it->tree_edges.clear();
      it->tree_incidents = 0;
      splay(it);
    }
    // try to reconnect
    for (auto it = base::find_first_after(nil, _has_edge_incidents, _has_edge_incidents);
         it != nil; it = base::find_first_after(it, _has_edge_incidents, _has_edge_incidents)) {
      int cur = it - &(base::data[0]);
      while (!it->edges.empty()) {
        auto e = it->edges.first();
        int neighbour = e->key >> 1;
        if (base::is_connected(cur, neighbour)) {
          splay(it)->del_edge(e);
          splay(neighbour)->del_edge_to(cur);
          forest->add_edge(cur, neighbour, false);
        } else {
          splay(it)->del_edge(e);
          splay(neighbour)->del_edge_to(cur);
          base::link(cur, neighbour);
          add_edge(cur, neighbour, true);
          return std::pair(cur, neighbour);
        }
      }
    }
    return std::pair(-1, -1);
    ;
  }

 private:
  static bool _has_edge_incidents(dynamic_connectivity_node_base* x) {
    return x->edge_incidents > 0;
  };
  static bool _has_tree_incidents(dynamic_connectivity_node_base* x) {
    return x->tree_incidents > 0;
  };
};

template <class node_t = dynamic_connectivity_node_simple, bool use_memory_pool = false>
struct dynamic_connectivity : dynamic_forest_layer<node_t, use_memory_pool> {
  using base = dynamic_forest_layer<node_t, use_memory_pool>;
  using base::nil, base::size;

  using forest_t = dynamic_forest_layer<dynamic_connectivity_node_simple, use_memory_pool>;
  std::vector<forest_t*> forest;
  dynamic_connectivity(int n) : base(n) {
    int lg_n = (n ? 32 - __builtin_clz(n) : 0);  // C++17 (int)bit_width((unsigned int)n);
    if (lg_n > 1) {
      forest.reserve(lg_n - 1);
      for (int i = 1; i < lg_n; i++) {
        forest.push_back(new forest_t(n));
      }
    }
  }

  void link(int u, int v) {
    bool separated = !base::is_connected(u, v);
    base::add_edge(u, v, separated);
    if (separated) {
      base::link(u, v);
    }
  }

  void cut(int u, int v) {
    // [reconnect?][didcut?]
    if (int cut_result = base::try_cut(u, v); cut_result) {
      if (cut_result & 0b10) {
        reconnect(u, v, -1);
      }
      return;
    }
    for (int i = 0; i < (int)forest.size(); i++) {
      if (int cut_result = forest[i]->try_cut(u, v); cut_result) {
        if (cut_result & 0b10) {
          base::cut(u, v);
          for (int j = 0; j < i; j++) {
            forest[j]->cut(u, v);
          }
          reconnect(u, v, i);
        }
        return;
      }
    }
  }

  void reconnect(int u, int v, int layer) {
    if (layer == -1) {
      base::reconnect(u, v, forest[layer + 1]);
    } else {
      auto [a, b] = forest[layer]->reconnect(u, v, forest[layer + 1]);
      if (a < 0 && b < 0) {
        reconnect(u, v, layer - 1);
      } else {
        base::link(a, b);
        for (int i = 0; i < layer; i++) {
          forest[i]->link(a, b);
        }
      }
    }
  }

  template <class... Args>
  void update_component(int v, const Args&... args) {
    base::update_all(v, args...);
  }

  template <class... Args>
  auto query_component(int v, const Args&... args) {
    return base::query_all(v, args...);
  }
};
