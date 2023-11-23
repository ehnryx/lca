/* Circular Buffer
 * USAGE
 *  circular_buffer<T> queue(n); // reserve n (optional)
 * MEMBERS
 *  void push_back(T);
 *  void push_front(T);
 *  void pop_back();
 *  void pop_front();
 *  T front();
 *  T back();
 * TIME
 *  O(1) push/pop/front/back
 * STATUS
 *  untested
 * SLOW!!!
 */
#pragma once

#include <array>
#include <stdexcept>

template <typename T>
struct circular_buffer_data {
  static constexpr int max_size_N = 29;
  virtual void push_front(const T& v) = 0;
  virtual void push_back(const T& v) = 0;
  virtual void pop_front() = 0;
  virtual void pop_back() = 0;
  virtual const T& front() const = 0;
  virtual const T& back() const = 0;
  virtual bool empty() const = 0;
  virtual int size() const = 0;
  virtual bool full() const = 0;
  virtual circular_buffer_data<T>* reallocate() = 0;
  virtual int capacity() const = 0;
  virtual ~circular_buffer_data() = default;
};

template <typename T, int N>
struct circular_buffer_data_N final : circular_buffer_data<T> {
  static constexpr int length = 1 << N;
  std::array<T, length> data;
  int fp, bp;  // [inclusive, exclusive)
  circular_buffer_data_N() : fp(0), bp(0) {}
  template <int M>
  circular_buffer_data_N(const circular_buffer_data_N<T, M>& old) : fp(0), bp(old.size()) {
    if (old.bp < old.fp) {
      memcpy(&data[0], &old.data[old.fp], (old.capacity() - old.fp) * sizeof(T));
      memcpy(&data[old.capacity() - old.fp], &old.data[0], old.bp * sizeof(T));
    } else {
      memcpy(&data[0], &old.data[old.fp], bp * sizeof(T));
    }
  }
  void push_front(const T& v) override {
    if (fp == 0) fp = capacity();
    data[--fp] = v;
  }
  void push_back(const T& v) override {
    data[bp++] = v;
    if (bp == capacity()) bp = 0;
  }
  void pop_front() override {
    if (++fp == capacity()) fp = 0;
  }
  void pop_back() override {
    if (--bp == -1) bp = capacity() - 1;
  }
  const T& front() const override { return data[fp]; }
  const T& back() const override { return bp ? data[bp - 1] : data[capacity() - 1]; }
  bool empty() const override { return fp == bp; }
  int size() const override { return bp < fp ? bp - fp + capacity() : bp - fp; }
  bool full() const override { return (bp + 1 == fp || (fp == 0 && bp == capacity() - 1)); }
  circular_buffer_data<T>* reallocate() override {
    if constexpr (N < circular_buffer_data<T>::max_size_N) {
      return new circular_buffer_data_N<T, N + 1>(*this);
    } else {
      throw std::runtime_error("can't increase circular buffer size");
    }
  }
  int capacity() const override { return length; }
};

// clang-format off
template <typename T>
circular_buffer_data<T>* make_circular_buffer_data(int n) {
  switch (n ? 32 - __builtin_clz(n) : 0) { // C++17 (int)bit_width((unsigned int)n);
    case 0://return new circular_buffer_data_N<T, 0>();
    case 1://return new circular_buffer_data_N<T, 1>();
    case 2:  return new circular_buffer_data_N<T, 2>();
    case 3:  return new circular_buffer_data_N<T, 3>();
    case 4:  return new circular_buffer_data_N<T, 4>();
    case 5:  return new circular_buffer_data_N<T, 5>();
    case 6:  return new circular_buffer_data_N<T, 6>();
    case 7:  return new circular_buffer_data_N<T, 7>();
    case 8:  return new circular_buffer_data_N<T, 8>();
    case 9:  return new circular_buffer_data_N<T, 9>();
    case 10: return new circular_buffer_data_N<T, 10>();
    case 11: return new circular_buffer_data_N<T, 11>();
    case 12: return new circular_buffer_data_N<T, 12>();
    case 13: return new circular_buffer_data_N<T, 13>();
    case 14: return new circular_buffer_data_N<T, 14>();
    case 15: return new circular_buffer_data_N<T, 15>();
    case 16: return new circular_buffer_data_N<T, 16>();
    case 17: return new circular_buffer_data_N<T, 17>();
    case 18: return new circular_buffer_data_N<T, 18>();
    case 19: return new circular_buffer_data_N<T, 19>();
    case 20: return new circular_buffer_data_N<T, 20>();
    case 21: return new circular_buffer_data_N<T, 21>();
    case 22: return new circular_buffer_data_N<T, 22>();
    case 23: return new circular_buffer_data_N<T, 23>();
    case 24: return new circular_buffer_data_N<T, 24>();
    case 25: return new circular_buffer_data_N<T, 25>();
    case 26: return new circular_buffer_data_N<T, 26>();
    case 27: return new circular_buffer_data_N<T, 27>();
    case 28: return new circular_buffer_data_N<T, 28>();
    case 29: return new circular_buffer_data_N<T, 29>();
    default: throw std::invalid_argument(
                 "can't allocate circular buffer of size " + std::to_string(n));
  }
}
// clang-format on

template <typename T>
struct circular_buffer {
  circular_buffer_data<T>* data;
  circular_buffer(int n = 0) : data(make_circular_buffer_data<T>(n)) {}
  void reallocate() {
    circular_buffer_data<T>* new_data = data->reallocate();
    delete data;
    data = new_data;
  }
  void push_front(const T& v) {
    if (data->full()) reallocate();
    data->push_front(v);
  }
  void push_back(const T& v) {
    if (data->full()) reallocate();
    data->push_back(v);
  }
  void pop_front() { data->pop_front(); }
  void pop_back() { data->pop_back(); }
  const T& front() const { return data->front(); }
  const T& back() const { return data->back(); }
  bool empty() const { return data->empty(); }
  int size() const { return data->size(); }
  int capacity() const { return data->capacity(); }
};
