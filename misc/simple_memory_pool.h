/* Simple (non-static) Memory Pool
 * USAGE
 *  pray
 * TIME
 *  O(1)
 * STATUS
 *  untested
 */
#pragma once

template <class T>
struct simple_memory_pool {
  vector<T> memory;
  vector<int> available;
  int pid;
  simple_memory_pool(int n): memory(n), available(n), pid(n) {
    iota(begin(available), end(available), 0);
  }
  T* allocate() {
    return &memory[available[--pid]];
  }
  void deallocate(T* x) {
    available[pid++] = (int)(x - &memory[0]);
  }
};
