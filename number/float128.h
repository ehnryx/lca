/* Utility for float128
 * USAGE
 *  don't
 * STATUS
 *  untested
 */
#pragma once

#include <iostream>

inline std::ostream& operator<<(std::ostream& os, const __float128& v) {
  return os << (long double)v;
}

inline std::istream& operator>>(std::istream& is, __float128& v) {
  double x;
  is >> x;
  v = x;
  return is;
}
