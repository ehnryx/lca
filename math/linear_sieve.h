/* Linear Sieve for Multiplicative Functions
 * USAGE
 *  linear_sieve<func> f(N);
 *  func should have the following members
 *    using T = output_type;
 *    static constexpr bool store_powers = true/false;
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
struct linear_sieve {
  using T = typename Func::T;
  static constexpr bool store_powers = Func::store_powers;
  using prime_t = conditional_t<store_powers, vector<int>, int>;
  vector<T> f;
  vector<prime_t> prime;
  linear_sieve(int n): f(n) {
    prime.reserve(n);
    vector<bool> composite(n);
    vector<int> cnt(n);
    f[1] = Func::one();
    for (int i = 2; i < n; i++) {
      if (!composite[i]) {
        if constexpr (store_powers) prime.emplace_back(1, i);
        else prime.push_back(i);
        f[i] = Func::prime(i);
        cnt[i] = 1;
      }
      if constexpr (store_powers) {
        for (auto& p : prime) {
          int ip = i * p.front();
          if (ip >= n) break;
          composite[ip] = true;
          if (i % p.front() == 0) {
            cnt[ip] = cnt[i] + 1;
            if (i == p.back()) {
              p.push_back(ip);
              f[ip] = Func::prime_power(f[i], p.front(), cnt[ip]);
            } else {
              f[ip] = Func::coprime(f[i / p[cnt[i] - 1]], f[p[cnt[i]]]);
            }
            break;
          } else {
            cnt[ip] = 1;
            f[ip] = Func::coprime(f[i], f[p.front()]);
          }
        }
      } else {
        for (int p : prime) {
          int ip = i * p;
          if (ip >= n) break;
          composite[ip] = true;
          if (i % p == 0) {
            cnt[ip] = cnt[i] + 1;
            f[ip] = Func::noncoprime(f[i], p, cnt[ip]);
            break;
          } else {
            cnt[ip] = 1;
            f[ip] = Func::coprime(f[i], f[p]);
          }
        }
      }
    }
  }
  T operator [] (int i) const { return f[i]; }
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

