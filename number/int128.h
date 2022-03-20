/* Utility for int128
 * USAGE
 *  don't
 * STATUS
 *  untested
 */
#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic" // ISO C++ __int128

__int128 gcd(const __int128& a, const __int128& b) {
  if(b == 0) return a < 0 ? -a : a;
  return gcd(b, a % b);
}

string to_string(const __int128& v) {
  bool neg = v < 0;
  string out;
  for(__int128 n = neg ? -v : v; n; n /= 10) {
    out.push_back((char)(n % 10 + '0'));
  }
  if(out.empty()) out.push_back('0');
  if(neg) out.push_back('-');
  reverse(begin(out), end(out));
  return out;
}

ostream& operator << (ostream& os, const __int128& v) {
  return os << to_string(v);
}

istream& operator >> (istream& is, __int128& v) {
  string s;
  is >> s;
  size_t i = 0;
  bool negative = (s[i] == '-');
  if (s[i] == '-' || s[i] == '+') i++;
  v = 0;
  for (; i < s.size(); i++) {
    if (!isdigit(s[i])) throw runtime_error("Error reading __int128");
    v = v * 10 + (s[i] - '0');
  }
  if (negative) v = -v;
  return is;
}

#pragma GCC diagnostic pop

