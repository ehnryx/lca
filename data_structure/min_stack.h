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
 *  tested: spoj/ARRAYSUB, spoj/LCS2
 */
#pragma once

template <typename T, class Compare = less<>>
struct min_stack : vector<pair<T, T>> {
  min_stack() = default;
  min_stack(int n) { vector<pair<T, T>>::reserve(n); }
  T min() const { return vector<pair<T, T>>::back().second; }
  T top() const { return vector<pair<T, T>>::back().first; }
  void pop() { vector<pair<T, T>>::pop_back(); }
  void push(const T& v) {
    vector<pair<T, T>>::emplace_back(v,
        vector<pair<T, T>>::empty() ? v :
        std::min(v, vector<pair<T, T>>::back().second, Compare()));
  }
};

