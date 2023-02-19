/* N-Dimensional Array
 * USAGE
 *  nd_array<int, 3> arr(n1, n2, n3, initial_value);
 *  nd_array<int, 3> arr(n1, n2, n3);
 *  arr(i, j, k) for access (instead of arr[i][j][k])
 *  arr.at(i, j, k) asserts indices in bounds (SLOW)
 *  arr.at_slow(i, j, k) throws if out of bounds (VERY VERY SLOW)
 *    dimensions 0-indexed from the right
 * NOTES
 *  stored in row-major order in a flattened vector
 *  unable to resize
 * STATUS
 *  untested
 */
#pragma once

#include <stdexcept>
#include <vector>

template <size_t dims>
struct nd_array_indexer : nd_array_indexer<dims - 1> {
  static_assert(dims > 1);
  using nested_indexer = nd_array_indexer<dims - 1>;
  const size_t _size;
  template <typename... Args>
  nd_array_indexer(size_t cur_dim, Args... ds): nested_indexer(ds...),
    _size(nested_indexer::size() * cur_dim) {}
  size_t size() const { return _size; }
  template <typename... Args>
  size_t get(size_t cur_i, Args... is) const {
    return cur_i * nested_indexer::size() + nested_indexer::get(is...);
  }
  template <typename... Args>
  size_t at(size_t cur_i, Args... is) const {
    size_t index = cur_i * nested_indexer::size() + nested_indexer::at(is...);
    assert(index < _size);
    return index;
  }
  template <typename... Args>
  size_t at_slow(size_t cur_i, Args... is) const {
    if (size_t index = cur_i * nested_indexer::size() + nested_indexer::at_slow(is...);
        index < _size) [[likely]] return index;
    throw std::invalid_argument("nd_array_indexer<" +
        std::to_string(dims) + "> out of bounds @ " + std::to_string(cur_i));
  }
};

template <>
struct nd_array_indexer<1> {
  const size_t _size;
  nd_array_indexer(size_t dim, ...): _size(dim) {}
  size_t size() const { return _size; }
  size_t get(size_t i) const { return i; }
  size_t at(size_t i) const {
    assert(i < _size);
    return i;
  }
  size_t at_slow(size_t i) const {
    if(i < _size) [[likely]] return i;
    throw std::invalid_argument(
        "nd_array_indexer<1> out of bounds @ " + std::to_string(i));
  }
};

template <typename T, size_t dims>
struct nd_array {
  const nd_array_indexer<dims> indexer;
  std::vector<T> data;
  template <typename... Args>
  nd_array(Args... ds): indexer(ds...), data(indexer.size(), _get_init(ds...)) {}
  size_t size() const { return indexer.size(); }

  // access
  template <typename... Args>
  T& operator()(Args... is) { return data[indexer.get(is...)]; }
  template <typename... Args>
  const T& operator()(Args... is) const { return data[indexer.get(is...)]; }

  // access with bounds checking
  template <typename... Args>
  T& at(Args... is) { return data[indexer.at(is...)]; }
  template <typename... Args>
  const T& at(Args... is) const { return data[indexer.at(is...)]; }
  template <typename... Args>
  T& at_slow(Args... is) { return data[indexer.at_slow(is...)]; }
  template <typename... Args>
  const T& at_slow(Args... is) const { return data[indexer.at_slow(is...)]; }

private:
  template <typename... Args>
  static T _get_init(Args... ds) {
    if constexpr (sizeof...(ds) == dims) {
      return T{};
    } else {
      static_assert(sizeof...(ds) == dims + 1);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
      return (ds, ...);
#pragma GCC diagnostic pop
    }
  }
};

