/* Simple (non-static) Memory Pool
 * USAGE
 *  pray
 * TIME
 *  O(1)
 * STATUS
 *  untested
 */
#pragma once

#include <numeric>
#include <vector>

template <class T>
struct simple_memory_pool {
  std::vector<T> memory;
  std::vector<int> available;
  int pid;
  simple_memory_pool(int n) : memory(n), available(n), pid(n) {
    std::iota(begin(available), end(available), 0);
  }
  T* allocate() { return &memory[available[--pid]]; }
  void deallocate(T* x) { available[pid++] = (int)(x - &memory[0]); }
};
