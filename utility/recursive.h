/* Recursive lambdas
 * USAGE
 *  auto foo = recursive([&](auto self, args...) {
 *    self(args2);
 *  }); foo(args);
 *  instead of
 *  auto foo = [&](auto self, args...) {
 *    self(self, args2);
 *  }); foo(foo, args);
 * STATUS
 *  tested: cf/104772cfi
 */
#pragma once

#include <functional>
#include <utility>

template <typename Function>
struct recursive_lambda_impl {
  Function f;
  recursive_lambda_impl(Function&& _f) : f(std::move(_f)) {}
  template <typename... Args>
  decltype(auto) operator()(Args&&... args) {
    return f(std::ref(*this), std::forward<Args>(args)...);
  }
};

template <typename Function>
auto recursive(Function&& f) {
  return recursive_lambda_impl<std::decay_t<Function>>(std::forward<Function>(f));
}
