/* N-Dimensional Indexer
 * USAGE
 *  idk
 */
#pragma once

#include <cassert>
#include <stdexcept>
#include <tuple>

template <size_t dims>
struct nd_indexer : nd_indexer<dims - 1> {
  static_assert(dims > 1);
  using nested_indexer = nd_indexer<dims - 1>;
  const size_t _size;
  template <typename... Args>
  nd_indexer(size_t cur_dim, Args... ds)
      : nested_indexer(ds...), _size(nested_indexer::size() * cur_dim) {}
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
        index < _size) [[likely]]
      return index;
    throw std::invalid_argument(
        "nd_indexer<" + std::to_string(dims) + "> out of bounds @ " +
        std::to_string(cur_i));
  }
  auto from_index(size_t i) const {
    size_t cur_i = i / nested_indexer::size();
    return std::tuple_cat(
        std::tuple(cur_i), nested_indexer::from_index(i - nested_indexer::size() * cur_i));
  }
};

template <>
struct nd_indexer<1> {
  const size_t _size;
  nd_indexer(size_t dim, ...) : _size(dim) {}
  size_t size() const { return _size; }
  size_t get(size_t i) const { return i; }
  size_t at(size_t i) const {
    assert(i < _size);
    return i;
  }
  size_t at_slow(size_t i) const {
    if (i < _size) [[likely]]
      return i;
    throw std::invalid_argument("nd_indexer<1> out of bounds @ " + std::to_string(i));
  }
  auto from_index(size_t i) const { return std::tuple(i); }
};
