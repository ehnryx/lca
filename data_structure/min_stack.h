/* Min Stack
 * USAGE
 *  min_stack<T> stk(n); // reserve n (optional)
 * MEMBERS
 *  void push(T);
 *  void pop();
 *  T top();
 *  T min();
 * TIME
 *  O(1) push/pop/min/top
 * STATUS
 *  tested: spoj/ARRAYSUB,LCS2
 */
#pragma once

#include <functional>
#include <vector>

template <typename T, class Compare = std::less<>>
struct min_stack {
  struct stack_item {
    T value, min;
  };
  std::vector<stack_item> stk;
  min_stack() = default;
  min_stack(int n) { stk.reserve(n); }
  T min() const { return stk.back().min; }
  T top() const { return stk.back().value; }
  void pop() { stk.pop_back(); }
  void push(const T& v) {
    stk.emplace_back(v, stk.empty() ? v : std::min(v, stk.back().min, Compare()));
  }
};
