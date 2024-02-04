/* Min Heap
 * USAGE
 *  heap<T> queue;
 * MEMBERS
 *  void push(T);
 *  void pop();
 *  T top();
 *  bool empty();
 *  int size();
 * TIME
 *  O(log) push/pop
 * STATUS
 *  boj/1753
 */
#pragma once

#include <functional>
#include <limits>
#include <vector>

template <typename T, class Compare = std::less<T>>
struct heap {
  std::vector<T> data;

  heap(int n = 0) { data.reserve(n); }
  template <typename InputIt>
  heap(InputIt first, InputIt last) : data(first, last) {
    _heapify(0);
  }
  heap(const std::vector<T>& v) : heap(v.begin(), v.end()) {}

  void push(const T& v) {
    data.push_back(v);
    _pull_up(data.size() - 1);
  }
  template <typename... Args>
  void emplace(Args&&... args) {
    data.emplace_back(std::forward<Args>(args)...);
    _pull_up(data.size() - 1);
  }
  void pop() {
    std::swap(data[0], data.back());
    data.pop_back();
    if (!data.empty()) _push_down(0);
  }

  const T& top() const { return data.front(); }
  bool empty() const { return data.empty(); }
  int size() const { return (int)data.size(); }

  std::vector<T> ordered(size_t limit = std::numeric_limits<size_t>::max()) const {
    if (empty()) return {};
    std::vector<T> res;
    heap<std::pair<T, size_t>> bfs;
    bfs.emplace(data.front(), 0);
    while (!bfs.empty()) {
      auto [value, index] = bfs.top();
      bfs.pop();
      res.push_back(move(value));
      if (res.size() == limit) return res;
      if (size_t left = 2 * index + 1; left < data.size()) {
        bfs.emplace(data[left], left);
      }
      if (size_t right = 2 * index + 2; right < data.size()) {
        bfs.emplace(data[right], right);
      }
    }
    return res;
  }

 private:
  void _heapify(size_t i) {
    if (i >= data.size()) return;
    size_t j = 2 * i + 1;
    _heapify(j);
    _heapify(j + 1);
    _push_down(i);
  }

  void _pull_up(size_t i) {
    while (i != 0) {
      size_t j = (i - 1) / 2;
      if (Compare()(data[i], data[j])) {
        std::swap(data[i], data[j]);
      } else {
        break;
      }
      i = j;
    }
  }

  void _push_down(size_t i) {
    for (size_t j = 2 * i + 1; j < data.size();) {
      bool right = (j + 1 < data.size() && Compare()(data[j + 1], data[j]));
      if (right && Compare()(data[j + 1], data[i])) {
        std::swap(data[i], data[j + 1]);
        i = j + 1;
      } else if (!right && Compare()(data[j], data[i])) {
        std::swap(data[i], data[j]);
        i = j;
      } else {
        break;
      }
      j = 2 * i + 1;
    }
  }
};
