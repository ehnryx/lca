/* Min Queue
 * USAGE
 *  min_queue<T> queue(n); // reserve n (optional)
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

#include "min_stack.h"

template <typename T, class Compare = std::less<>>
struct min_queue {
  min_stack<T, Compare> in, out;
  min_queue() = default;
  min_queue(int n): in(n), out(n) {}
  void refill() {
    while (!in.empty()) {
      out.push(in.top());
      in.pop();
    }
  }
  T min() const {
    if (in.empty()) return out.min();
    if (out.empty()) return in.min();
    return std::min(in.min(), out.min(), Compare());
  }
  T top() const {
    if (out.empty()) refill();
    return out.top();
  }
  void pop() {
    if (out.empty()) refill();
    out.pop();
  }
  void push(const T& v) {
    in.push(v);
  }
};

