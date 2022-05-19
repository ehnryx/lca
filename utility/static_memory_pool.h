/* (Simple) Static Memory Pool
 * USAGE
 *  pray
 * TIME
 *  O(1)
 * STATUS
 *  untested
 */
#pragma once

#include <array>

template <class T, int N>
struct static_memory_pool {
  std::array<T, N> memory;
  std::array<int, N> available;
  int pid;
  static_memory_pool(): pid(N) {
    iota(begin(available), end(available), 0);
  }
  T* allocate() {
    return &memory[available[--pid]];
  }
  void deallocate(T* x) {
    available[pid++] = (int)(x - &memory[0]);
  }
};
