/* Ternary Search
 * USAGE
 *  ternary_search(left, right, function);
 * TIME
 *  O(log)
 * STATUS
 *  tested: cf/439d
 */
#pragma once

#include <numbers>
#include <optional>
#include <type_traits>
#include <utility>

template <typename index_t, typename Function, typename Compare>
  requires(std::is_integral_v<index_t>)
auto ternary_search(index_t left, index_t right, Function const& f, Compare const& cmp) {
  while (left + 2 < right) {
    index_t const mid = (left + right) / 2;
    if (cmp(f(mid), f(mid + 1))) {
      right = mid + 1;
    } else {
      left = mid;
    }
  }
  index_t index = right;
  auto value = f(right);
  for (index_t i = right - 1; i >= left; i--) {
    auto const cur = f(i);
    if (cmp(cur, value)) {
      index = i;
      value = std::move(cur);
    }
  }
  return std::pair(std::move(value), index);
}

template <typename index_t, typename Function, typename Compare>
  requires(std::is_floating_point_v<index_t>)
auto ternary_search(
    index_t left, index_t right, int iters, Function const& f, Compare const& cmp) {
  static constexpr auto weight = 1 / std::numbers::phi_v<index_t>;
  using value_t = decltype(std::declval<Function>()(std::declval<index_t>()));
  std::optional<value_t> left_value, right_value;
  while (iters--) {
    index_t const left_mid = weight * left + (1 - weight) * right;
    index_t const right_mid = (1 - weight) * left + weight * right;
    if (left_mid <= left or right <= right_mid) break;
    if (not left_value) left_value = f(left_mid);
    if (not right_value) right_value = f(right_mid);
    if (cmp(*left_value, *right_value)) {
      right = right_mid;
      right_value = left_value;
      left_value.reset();
    } else {
      left = left_mid;
      left_value = right_value;
      right_value.reset();
    }
  }
  index_t const index = (left + right) / 2;
  return std::pair(f(index), index);
}
