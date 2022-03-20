/* Heap
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
 *  untested
 */
#pragma once

template <typename T, class Compare = less<>>
struct heap {
  vector<T> data;

  heap(int n = 0) {
    data.reserve(n);
  }
  template <typename InputIt>
  heap(InputIt first, InputIt last): data(first, last) {
    heapify(0);
  }
  heap(const vector<T>& v): heap(v.begin(), v.end()) {}

  void push(const T& v) {
    data.push_back(v);
    _pull_up(data.size() - 1);
  }

  template <typename... Args>
  void emplace(Args&&... args) {
    data.emplace_back(forward<Args>(args)...);
    _pull_up(data.size() - 1);
  }

  void pop() {
    swap(data[0], data.back());
    data.pop_back();
    if (!data.empty()) _push_down(0);
  }

  const T& top() const { return data.front(); }
  bool empty() const { return data.empty(); }
  int size() const { return (int)data.size(); }

  void heapify(size_t i) {
    size_t j = 2 * i;
    if (j < data.size()) {
      heapify(j);
      if (j + 1 < data.size()) {
        heapify(j + 1);
      }
    }
    _push_down(i);
  }

  vector<T> ordered(size_t limit = numeric_limits<size_t>::max()) const {
    if (empty()) return {};
    vector<T> res;
    heap<pair<T, size_t>> bfs;
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
  void _pull_up(size_t i) {
    while (i != 0) {
      size_t j = (i - 1) / 2;
      if (Compare()(data[i], data[j])) {
        swap(data[i], data[j]);
      } else {
        break;
      }
      i = j;
    }
  }

  void _push_down(size_t i) {
    for (size_t j = 2 * i + 1; j < data.size(); ) {
      bool right = (j + 1 < data.size() && Compare()(data[j + 1], data[j]));
      if (right && Compare()(data[j + 1], data[i])) {
        swap(data[i], data[j + 1]);
        i = j + 1;
      } else if (!right && Compare()(data[j], data[i])) {
        swap(data[i], data[j]);
        i = j;
      } else {
        break;
      }
      j = 2 * i + 1;
    }
  }
};

