/* Fake Memory Pool
 * USAGE
 *  don't
 * TIME
 *  O(1)
 * STATUS
 *  untested
 */
#pragma once

template <class T>
struct fake_memory_pool {
  fake_memory_pool() {}
  fake_memory_pool(int) {}
  static T* allocate() { return new T; }
  static void deallocate(T* x) { delete x; }
};
