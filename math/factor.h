/* Integer Factoring
 * USAGE
 *  prime_factor<F, T>(n); -> vector<pair<T, int>> : [(prime factor, count)]
 *  get_divisors<F, T>(n); -> vector<T> : [divisor]
 *  F uses pollard_rho and millar_rabin by default
 *  F::get_factor(n), F::is_prime need to be defined.
 *  T is deduced from n
 * TIME
 *  O(???)
 * STATUS
 *  untested
 */
#pragma once

#include "miller_rabin.h"
#include "pollard_rho.h"

#include <utility>
#include <vector>

namespace factor_details {
struct factor {
  template <typename T>
  static auto get_factor(T n) {
    return pollard_rho::get_factor(n);
  }
  template <typename T>
  static auto is_prime(T n) {
    return miller_rabin::is_prime(n);
  }
};
}  // namespace factor_details

template <typename Factor = factor_details::factor, typename value_t>
std::vector<std::pair<value_t, int>> prime_factor(value_t n) {
  std::vector<std::pair<value_t, int>> factors;
  while (n > 1) {
    value_t k = Factor::get_factor(n);
    while (not Factor::is_prime(k)) k = Factor::get_factor(k);
    int count = 0;
    while (n % k == 0) {
      n /= k;
      count += 1;
    }
    factors.emplace_back(k, count);
  }
  return factors;
}

enum class sort_factors {
  No = 0,
  Yes = 1,
};

template <typename Factor = factor_details::factor, typename value_t>
std::vector<value_t> get_divisors(value_t n, sort_factors s = sort_factors::Yes) {
  auto const prime_factors = prime_factor<Factor>(n);
  int num_divisors = 1;
  for (auto const& [_, count] : prime_factors) {
    num_divisors *= count + 1;
  }
  std::vector<value_t> divisors;
  divisors.reserve(num_divisors);
  divisors.push_back(1);
  for (auto const& [pf, count] : prime_factors) {
    int const have = divisors.size();
    for (int i = 0; i < count * have; i++) {
      divisors.push_back(divisors[i] * pf);
    }
  }
  if (s == sort_factors::Yes) {
    std::sort(divisors.begin(), divisors.end());
  }
  return divisors;
}
