/* Overload
 * USAGE
 *  auto foo = overload{f1, f2, ...};  // uses aggregate init
 *  foo(args1); foo(args2); calls correct function obj based on overload
 * STATUS
 *  untested
 */
#pragma once

#include <utility>

template <typename... Fs>
struct overload : Fs... {
  using Fs::operator()...;
};
