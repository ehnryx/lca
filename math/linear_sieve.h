/* Linear Sieve for Multiplicative Functions
 * USAGE
 *  linear_sieve<func, store_powers> f(N);
 *  store_powers? is a bool specifying whether prime powers should be stored
 *  func should have the following members
 *    using T = output_type;
 *    static T one(); // f(1) = 1
 *    static T coprime(T f(v), T f(p)); // f(vp) = f(v)f(p) for (v, p) = 1
 *    static T prime(int p); // f(p)
 *    if store_powers:
 *      static T prime_power(T f(p^{k-1}), int p, int k); // f(p^k) for k > 1
 *    otherwise:
 *      static T noncoprime(T f(v), int p, int k); // f(vp) for p|v, p^k||vp
 * TIME
 *  O(N)
 * STATUS
 *  tested: kattis/nonprimefactors
 */
#pragma once

template <class Func>
struct linear_sieve_base {
  using T = typename Func::T;
  vector<T> f;
  vector<int> cnt;
  linear_sieve_base(int n): f(n), cnt(n) {}
  T operator[](int i) const { return f[i]; }
};

template <>
struct linear_sieve_base<void> {
  linear_sieve_base(int) {}
};

template <class Func = void, bool store_powers = false>
struct linear_sieve : linear_sieve_base<Func> {
  using base = linear_sieve_base<Func>;
  static constexpr bool has_function = !is_same_v<Func, void>;
  using prime_t = conditional_t<store_powers, vector<int>, int>;
  vector<bool> composite;
  vector<prime_t> prime;
  linear_sieve(int n): linear_sieve_base<Func>(n), composite(n) {
    prime.reserve(n);
    if constexpr (has_function) base::f[1] = Func::one();
    for (int i = 2; i < n; i++) {
      if (!composite[i]) {
        if constexpr (store_powers) prime.emplace_back(1, i);
        else prime.push_back(i);
        if constexpr (has_function) {
          base::f[i] = Func::prime(i);
          base::cnt[i] = 1;
        }
      }
      if constexpr (store_powers) {
        for (auto& p : prime) {
          int ip = i * p.front();
          if (ip >= n) break;
          composite[ip] = true;
          if (i % p.front() == 0) {
            if constexpr (has_function) base::cnt[ip] = base::cnt[i] + 1;
            if (i == p.back()) {
              p.push_back(ip);
              if constexpr (has_function) {
                base::f[ip] = Func::prime_power(base::f[i], p.front(), base::cnt[ip]);
              }
            } else {
              if constexpr (has_function) {
                base::f[ip] = Func::coprime(
                  base::f[i / p[base::cnt[i] - 1]], base::f[p[base::cnt[i]]]);
              }
            }
            break;
          } else if constexpr (has_function) {
            base::cnt[ip] = 1;
            base::f[ip] = Func::coprime(base::f[i], base::f[p.front()]);
          }
        }
      } else {
        for (int p : prime) {
          int ip = i * p;
          if (ip >= n) break;
          composite[ip] = true;
          if constexpr (has_function) {
            if (i % p == 0) {
              base::cnt[ip] = base::cnt[i] + 1;
              base::f[ip] = Func::noncoprime(base::f[i], p, base::cnt[ip]);
              break;
            } else {
              base::cnt[ip] = 1;
              base::f[ip] = Func::coprime(base::f[i], base::f[p]);
            }
          }
        }
      }
    }
  }
};

namespace multiplicative_functions {
  struct totient {
    using T = int;
    static constexpr bool store_powers = false;
    static T one() { return 1; }
    static T coprime(T fv, T fp) { return fv * fp; }
    static T prime(int p) { return p - 1; }
    static T noncoprime(int fp, int p, int) { return fp * p; }
  };

  struct mobius {
    using T = int;
    static constexpr bool store_powers = false;
    static T one() { return 1; }
    static T coprime(T fv, T fp) { return fv * fp; }
    static T prime(int) { return -1; }
    static T noncoprime(int, int, int) { return 0; }
  };

  struct divisor_num {
    using T = int;
    static constexpr bool store_powers = false;
    static T one() { return 1; }
    static T coprime(T fv, T fp) { return fv * fp; }
    static T prime(int) { return 2; }
    static T noncoprime(int fp, int, int k) { return fp / k * (k + 1); }
  };

  template <int power = 1, typename sum_t = int>
  struct divisor {
    using T = sum_t;
    static constexpr bool store_powers = true;
    static T one() { return 1; }
    static T coprime(T fv, T fp) { return fv * fp; }
    static T prime(int p) { return 1 + (T)powl(p, power); }
    static T prime_power(int fp, int p, int k) { return fp + (T)powl(p, k * power); }
  };
}
