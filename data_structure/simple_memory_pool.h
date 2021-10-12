/* Simple Memory Pool
 * USAGE
 *  pray
 * TIME
 *  O(1)
 * STATUS
 *  untested
 */
#pragma once

template <class T, int N>
struct simple_memory_pool {
  array<T, N> memory;
  array<int, N> available;
  int pid;
  simple_memory_pool() {
    iota(begin(available), end(available), 0);
    pid = N;
  }
  T* allocate() {
    return &memory[available[--pid]];
  }
  void deallocate(T* x) {
    available[pid++] = (int)(x - &memory[0]);
  }
};
