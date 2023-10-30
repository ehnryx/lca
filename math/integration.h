/* Numberical Integration
 * USAGE
 *  ans = integrate2(f, l, r, n = #steps);
 *  ans = integrate4(f, l, r, n = #steps);
 * TIME
 *  O(n * f)
 * STATUS
 *  untested: kattis/grazedgrains
 */
#pragma once

// Simpson's 1/3 rule
template <typename T, typename Function>
T integrate2(const Function& f, T left, T right, int steps) {
  T h = (right - left) / steps / 2;
  T ans = 0;
  T f0 = f(left);
  for (int i = 1; i <= steps; i++) {
    T x = ((steps - i) * left + i * right) / steps;
    T f1 = f(x - h);
    T f2 = f(x);
    ans += f0 + 4*f1 + f2;
    f0 = f2;
  }
  return ans * h / 3;
}

// Boole's rule
template <typename T, typename Function>
T integrate4(const Function& f, T left, T right, int steps) {
  T h = (right - left) / steps / 4;
  T ans = 0;
  T f0 = f(left);
  for (int i = 1; i <= steps; i++) {
    T x = ((steps - i) * left + i * right) / steps;
    T f1 = f(x - h * 3);
    T f2 = f(x - h * 2);
    T f3 = f(x - h);
    T f4 = f(x);
    ans += 7*(f0 + f4) + 32*(f1 + f3) + 12*f2;
    f0 = f4;
  }
  return ans * h * 2 / 45;
}
