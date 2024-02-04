/* Mo's algorithm
 * USAGE
 *  mo_solver<T, ...> solver(l, r);  // bounds of indices
 *  solver.add_event(l, r, t, ...);  // types given in decl
 *  solver.solve(update_func, query_func);  // run it
 *  // calls update_func(idx, +/-1, t, ...); when moving to events
 *  // calls query_func(t, ...); at events
 * TIME
 *  O(NsqrtN + M)
 *  N = |array|, M = |events|
 * STATUS
 *  untested
 */
#pragma once

#include <cassert>
#include <cmath>
#include <functional>
#include <tuple>
#include <vector>

template <typename... T>
struct mo_solver {
  using tuple_indices = std::index_sequence_for<T...>;
  struct item {
    int l, r;
    std::tuple<T...> data;
  };
  std::vector<item> events;
  const int start_index;
  const int block_size;
  mo_solver(int l, int r, int block = 0)
      : start_index(l), block_size(block ? block : (int)std::sqrt(r - l + 1)) {
    assert(l <= r);
    assert(block_size > 0);
  }
  void add_event(int l, int r, const T&... data) { events.emplace_back(l, r, tuple(data...)); }
  template <typename update_t, typename query_t>
  void solve(update_t&& update, query_t&& query) {
    solve(std::forward<update_t>(update), std::forward<query_t>(query), tuple_indices{});
  }
  template <typename update_t, typename query_t, size_t... I>
  void solve(update_t&& update, query_t&& query, std::index_sequence<I...>) {
    std::sort(events.begin(), events.end(), [this](const item& a, const item& b) {
      int block_a = (a.l - start_index) / block_size;
      int block_b = (b.l - start_index) / block_size;
      if (block_a != block_b) return block_a < block_b;
      return (block_a % 2 == 0 ? a.r < b.r : b.r < a.r);
    });
    int x = start_index;
    int y = start_index - 1;
    for (const auto& [l, r, data] : events) {
      while (y < r) std::invoke(update, ++y, 1, get<I>(data)...);
      while (x > l) std::invoke(update, --x, 1, get<I>(data)...);
      while (y > r) std::invoke(update, y--, -1, get<I>(data)...);
      while (x < l) std::invoke(update, x++, -1, get<I>(data)...);
      std::invoke(query, get<I>(data)...);
    }
  }
};
