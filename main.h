#pragma once

#ifdef PRINT_TIMING
#include <chrono>
#endif
#include <iomanip>
#include <iostream>

template <typename input_t>
struct solve_main_wrapper {
  input_t& cin;
  solve_main_wrapper(input_t& _cin, int argc, char** argv) : cin(_cin) {
    (void)argc;
    (void)argv;
  }
  void solve_main(int testnum);
  int solve_all() {
    int test_cases = 1;
#if defined(MULTI_TEST) or defined(PRINT_CASE)
    cin >> test_cases;
#endif
    for (int testnum = 1; testnum <= test_cases; testnum++) {
#define MAKE_STRING_IMPL(STRING) #STRING
#define MAKE_STRING(STRING) MAKE_STRING_IMPL(STRING)
#ifdef PRINT_CASE
      std::cout << "Case " << MAKE_STRING(PRINT_CASE) << testnum << ": ";
#undef MAKE_STRING
#undef MAKE_STRING_IMPL
#endif
#ifdef PRINT_TIMING
      auto start_time = std::chrono::high_resolution_clock::now();
#endif
      solve_main(testnum);
#ifdef PRINT_TIMING
      auto duration = std::chrono::high_resolution_clock::now() - start_time;
      using namespace std::chrono;
      std::cerr << "\n[t" << testnum << "] " << duration / 1.0s << "s\n\n";
#endif
    }
    return 0;
  }
};

int main(int argc, char** argv) {
  std::cout << std::fixed << std::setprecision(10);
#ifdef USING_FAST_INPUT
  fast_input cin;
  solve_main_wrapper solver(cin, argc, argv);
#else
  std::cin.tie(0)->sync_with_stdio(0);
  solve_main_wrapper solver(std::cin, argc, argv);
#endif
  return solver.solve_all();
}

#define SOLVE() \
  template <typename input_t> \
  void solve_main_wrapper<input_t>::solve_main([[maybe_unused]] int testnum)

#include <random>
inline auto rng() -> std::mt19937& {
  static std::random_device _rd;
  static std::mt19937 _rng(_rd());
  return _rng;
}

using ll = long long;
constexpr char nl = '\n';

#include <cassert>
using namespace std;
