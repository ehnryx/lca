/* Big Integer (radix 1e6)
 * USAGE
 *  int_base10 a(int, long long, string);
 *  supports operators +, -, *, /, %
 *  supports cin >> and cout <<
 * NOTE
 *  seems to be faster than java's BigInteger
 * STATUS
 *  mostly untested
 */
#pragma once

#include "../math/fast_fourier_transform.h"

struct int_base10 {
  static constexpr int radix = 10'000;
  static constexpr int radix_length = 4;
  static constexpr size_t fft_threshold = 144;
  static constexpr size_t small_div_threshold = 100;
  static constexpr size_t large_div_threshold = 144;
  static constexpr size_t recursive_div_limit = 64;
  static_assert(recursive_div_limit < small_div_threshold);

  bool negative;
  vector<int> digits;
  int operator [] (size_t i) const { return i < digits.size() ? digits[i] : 0; }
  bool is_zero() const { return digits.size() == 1 && digits[0] == 0; }
  bool is_one() const { return digits.size() == 1 && digits[0] == 1; }
  int_base10& clear() {
    negative = false;
    digits.resize(1);
    digits[0] = 0;
    return *this;
  }

  int_base10(): negative(false), digits(1, 0) {}
  int_base10(vector<int>&& v, bool neg = false): negative(neg), digits(move(v)) {
    fix_leading_zeros();
  }
  int_base10(const vector<int>& v, bool neg = false): negative(neg), digits(v) {
    fix_leading_zeros();
    for (int d : digits) {
      if (d < 0 || radix <= d) {
        throw invalid_argument("digit not in range [0, " + to_string(radix) + ")");
      }
    }
  }
  int_base10(int n): negative(n < 0) {
    if (negative) {
      while (n <= -radix) {
        digits.push_back(-(n % radix));
        n /= radix;
      }
      digits.push_back(-n);
    } else {
      while (n >= radix) {
        digits.push_back(n % radix);
        n /= radix;
      }
      digits.push_back(n);
    }
  }
  int_base10(long long n): negative(n < 0) {
    if (negative) {
      while (n <= -radix) {
        digits.push_back(-(int)(n % radix));
        n /= radix;
      }
      digits.push_back(-(int)n);
    } else {
      while (n >= radix) {
        digits.push_back((int)(n % radix));
        n /= radix;
      }
      digits.push_back((int)n);
    }
  }

  int_base10(const string& s) {
    build(s);
  }
  void build(const string& s) {
    negative = (s.front() == '-');
    int len = (int)s.size() - negative;
    int num_digits = (len + radix_length - 1) / radix_length;
    digits.resize(num_digits);
    for (int i = 0; i < num_digits; i++) {
      int start = i * radix_length;
      for (int j = 0, tens = 1; j < radix_length && start + j < len; j++) {
        if (j > 0) tens *= 10;
        char d = s[(int)s.size() - 1 - start - j];
        if (!isdigit(d)) throw invalid_argument("encountered non-digit character");
        digits[i] += (d - '0') * tens;
      }
    }
    fix_leading_zeros();
  }

  int length() const {
    int i = 1;
    for (int tens = 10; tens < digits.back(); tens *= 10) {
      i++;
    }
    return i + ((int)digits.size() - 1) * radix_length;
  }
  friend ostream& operator << (ostream& os, const int_base10& v) {
    assert(!v.digits.empty());
    stringstream out;
    if (v.negative) out << '-';
    out << v.digits.back();
    for (int i = (int)v.digits.size() - 2; i >= 0; i--) {
      out << setfill('0') << setw(radix_length) << v.digits[i];
    }
    return os << out.rdbuf();
  }
  friend istream& operator >> (istream& is, int_base10& v) {
    string s;
    is >> s;
    v.build(s);
    return is;
  }
  string str() const {
    ostringstream out;
    out << *this;
    return out.str();
  }
  friend string to_string(const int_base10& v) { return v.str(); }

  template <typename T>
  T value() const {
    T res(0), mul(1);
    for (int i = 0; i < digits.size(); i++) {
      if (i > 0) mul *= radix;
      res += digits[i] * mul;
    }
    return res;
  }
  bool abs_less(const int_base10& o) const {
    if (digits.size() != o.digits.size()) return digits.size() < o.digits.size();
    for (int i = (int)digits.size() - 1; i >= 0; i--) {
      if (digits[i] != o.digits[i]) {
        return digits[i] < o.digits[i];
      }
    }
    return false;
  }
  bool operator == (const int_base10& o) const {
    return negative == o.negative && digits == o.digits;
  }
  bool operator < (const int_base10& o) const {
    if (negative != o.negative) return negative;
    return negative ? o.abs_less(*this) : abs_less(o);
  }
  bool operator != (const int_base10& o) const { return !operator == (o); }
  bool operator > (const int_base10& o) const { return o.operator < (*this); }
  bool operator <= (const int_base10& o) const { return !o.operator < (*this); }
  bool operator >= (const int_base10& o) const { return !operator < (o); }

  friend int_base10 abs(const int_base10& v) {
    return v.negative ? int_base10(v).negate() : v;
  }
  friend int_base10 gcd(const int_base10& a, const int_base10& b) {
    if (a.is_zero()) return abs(b);
    return gcd(b % a, a);
  }
  friend int_base10 lcm(const int_base10& a, const int_base10& b) {
    return a * b / gcd(a, b);
  }

  int_base10 operator + (const int_base10& o) const { return int_base10(*this) += o; }
  int_base10 operator - (const int_base10& o) const { return int_base10(*this) -= o; }
  int_base10& operator *= (const int_base10& o) { return *this = operator * (o); }
  int_base10& operator /= (const int_base10& o) { return *this = operator / (o); }
  int_base10& operator %= (const int_base10& o) { return *this = operator % (o); }
  int_base10 operator - () const { return int_base10(*this).negate(); }
  int_base10& negate() {
    negative = !negative;
    return *this;
  }

  // shift by radix
  int_base10& operator <<= (int shift) { return left_shift(shift); }
  int_base10& operator >>= (int shift) { return right_shift(shift); }
  int_base10 operator << (int shift) const { return int_base10(*this) <<= shift; }
  int_base10 operator >> (int shift) const { return int_base10(*this) >>= shift; }

  int_base10& operator += (const int_base10& o) {
    if (negative != o.negative) return operator -= (-o);
    size_t len = max(digits.size(), o.digits.size());
    digits.resize(len);
    int carry = 0;
    for (size_t i = 0; i < o.digits.size(); i++) {
      digits[i] += carry + o.digits[i];
      carry = (digits[i] >= radix);
      if (carry) {
        digits[i] -= radix;
      }
    }
    for (size_t i = o.digits.size(); i < len && carry; i++) {
      digits[i] += carry;
      carry = (digits[i] >= radix);
      if (carry) {
        digits[i] -= radix;
      }
    }
    if (carry) {
      digits.push_back(1);
    }
    return *this;
  }

  int_base10& operator -= (const int_base10& o) {
    if (negative != o.negative) return operator += (-o);
    if (abs_less(o)) return *this = (o - *this).negate();
    int borrow = 0;
    for (size_t i = 0; i < o.digits.size(); i++) {
      digits[i] -= borrow + o.digits[i];
      borrow = (digits[i] < 0);
      if (borrow) {
        digits[i] += radix;
      }
    }
    for (size_t i = o.digits.size(); i < digits.size() && borrow; i++) {
      digits[i] -= borrow;
      borrow = (digits[i] < 0);
      if (borrow) {
        digits[i] += radix;
      }
    }
    fix_leading_zeros();
    negative &= !is_zero();
    return *this;
  }

  int_base10 operator * (const int_base10& o) const {
    if (is_zero() || o.is_zero()) return int_base10(0);
    if (is_one()) return o;
    if (o.is_one()) return *this;
    if (min(digits.size(), o.digits.size()) >= fft_threshold) return fft_multiply(o);
    vector<int> num(digits.size() + o.digits.size());
    for (size_t i = 0; i < digits.size(); i++) {
      if (digits[i] == 0) continue;
      for (size_t j = 0; j < o.digits.size(); j++) {
        long long val = num[i + j] + (long long)digits[i] * o.digits[j];
        num[i + j] = (int)(val % radix);
        num[i + j + 1] += (int)(val / radix);
      }
    }
    return int_base10(move(num), negative ^ o.negative);
  }

  int_base10 fft_multiply(const int_base10& o) const {
    vector<long long> num = convolve<long long, double>(digits, o.digits);
    vector<int> res(num.size() + 1);
    for (size_t i = 0; i < num.size(); i++) {
      num[i] += res[i];
      if (num[i] < radix) {
        res[i] = (int)num[i];
        res[i + 1] = 0;
      } else {
        res[i] = (int)(num[i] % radix);
        res[i + 1] = (int)(num[i] / radix);
      }
    }
    return int_base10(move(res), negative ^ o.negative);
  }

  int_base10 pow(const int_base10& exponent) const {
    if (exponent.negative) throw invalid_argument("exponent should not be negative");
    if (exponent.is_zero()) return int_base10(1);
    int_base10 res(1), base(*this);
    for (size_t i = 0; i < exponent.digits.size(); i++) {
      if (exponent[i] > 0) {
        res *= base.pow(exponent[i]);
      }
      if (i + 1 < exponent.digits.size()) {
        base = base.pow(radix);
      }
    }
    return res;
  }

  int_base10 pow(int exponent) const {
    if (exponent < 0) throw invalid_argument("exponent should not be negative");
    if (exponent == 0) return int_base10(1);
    int_base10 res(1), base(*this);
    while (exponent > 0) {
      if (exponent % 2) {
        res *= base;
      }
      exponent /= 2;
      if (exponent > 0) {
        base *= base;
      }
    }
    return res;
  }

  int_base10 operator / (const int_base10& o) const {
    int_base10 res = divide_remainder(o).first;
    res.negative ^= o.negative;
    return res;
  }
  int_base10 operator % (const int_base10& o) const {
    //if (o.negative) throw invalid_argument("mod should not be negative");
    int_base10 res = divide_remainder(o).second;
    res.negative ^= o.negative;
    return res;
  }

private:
  pair<int_base10, int_base10> divide_remainder(const int_base10& o) const {
    if (o.is_zero()) throw invalid_argument("division by zero");
    if (is_zero()) return pair(int_base10(0), int_base10(0));
    if (abs_less(o)) return pair(int_base10(0), *this);
    if (o.digits.size() > small_div_threshold
        && digits.size() - o.digits.size() > large_div_threshold) {
      return burnikel_ziegler(o);
    } else {
      return long_division(o);
    }
  }

  pair<int_base10, int_base10> long_division(const int_base10& o) const {
    if (o.digits.size() == 1) {
      return long_division_digit(digits, o.digits[0]);
    }
    int normalize = (radix / 2 + o.digits.back() - 1) / o.digits.back();
    vector<int> top = (operator * (normalize)).digits;
    vector<int> div = (o * normalize).digits;
    assert(div.size() == o.digits.size());
    size_t n = div.size();
    size_t m = digits.size() - n;
    if (top.size() == n + m) top.push_back(0);
    vector<int> res(m + 1);
    for (int i = (int)m; i >= 0; i--) {
      long long val = (long long)top[i + n] * radix + top[i + n - 1];
      int quotient = (int)(val / div.back());
      int remainder = (int)(val - (long long)quotient * div.back());
      while (remainder < radix &&
             (quotient == radix ||
              (long long)quotient * div[n - 2] > (long long)radix * remainder + top[i + n - 2])) {
        quotient -= 1;
        remainder += div.back();
      }
      int carry = 0;
      for (size_t j = 0; j < n; j++) {
        long long sub = carry + (long long)div[j] * quotient;
        carry = (int)(sub / radix);
        top[i + j] -= (int)(sub % radix);
        if (top[i + j] < 0) {
          top[i + j] += radix;
          top[i + j + 1] -= 1;
        }
      }
      top[i + n] -= carry;
      if (top[i + n] < 0) {
        for (size_t j = 0; j < n; j++) {
          top[i + j] += div[j];
          if (top[i + j] >= radix) {
            top[i + j] -= radix;
            top[i + j + 1] += 1;
          }
        }
        assert(0 <= top[i + n] && top[i + n] < radix);
        quotient -= 1;
      }
      res[i] = quotient;
    }
    return pair(
      move(int_base10(move(res))),
      move(long_division_digit(top, normalize).first));
  }

  // long_division_digit requires the numerator to be nonzero.
  pair<int_base10, int_base10> long_division_digit(const vector<int>& top, int div) const {
    if (div == 1) return pair(move(top), move(vector<int>(1, 0)));
    vector<int> res(top.size());
    int remainder = 0;
    for (size_t i = 0; i < top.size(); i++) {
      long long val = (long long)remainder * radix + top[top.size() - i - 1];
      int quotient = (int)(val / div);
      remainder = (int)(val - (long long)quotient * div);
      res[top.size() - 1 - i] = quotient;
    }
    return pair(
      move(int_base10(move(res))),
      move(int_base10(remainder)));
  }

  // burnikel-ziegler division
  pair<int_base10, int_base10> burnikel_ziegler(const int_base10& o) const {
    int normalize = (radix / 2 + o.digits.back() - 1) / o.digits.back();
    size_t n = o.digits.size() == 1 ? 1 : 1 << (32 - __builtin_clz((int)o.digits.size() - 1));
    size_t shift = n - o.digits.size();
    vector<int> top = (operator * (normalize)).left_shift(shift).digits;
    int_base10 div = (o * normalize).left_shift(shift);
    div.negative = false;
    size_t blocks = (top.size() + n - 1) / n;
    if (blocks < 2 || (blocks * n == top.size() && top.back() >= radix / 2)) {
      blocks += 1;
    }

    vector<int> res((blocks - 1) * n);
    vector<int> upper(top.begin() + (blocks - 1) * n, top.end());
    for (int i = (int)blocks - 2; i >= 0; i--) {
      vector<int> lower(top.begin() + i * n, top.begin() + (i + 1) * n);
      auto [quotient, remainder] = divide_2by1(upper, lower, div);
      upper = move(remainder.digits);
      copy(quotient.digits.begin(), quotient.digits.end(), res.begin() + i * n);
    }
    return pair(
      move(int_base10(move(res))),
      move(int_base10(move(upper)).right_shift(shift)));
  }

  pair<int_base10, int_base10> divide_2by1(
    vector<int>& upper, vector<int>& lower, const int_base10& div) const {
    if (div.digits.size() <= recursive_div_limit) {
      assert(lower.size() == div.digits.size());
      lower.insert(lower.end(), upper.begin(), upper.end());
      return int_base10(move(lower)).divide_remainder(div);
    }

    size_t n = div.digits.size() / 2;
    assert(n * 2 == div.digits.size());
    vector<int> lower_hi(lower.begin() + n, lower.end());
    lower.resize(n);
    auto [quotient_hi, remainder_hi] = divide_3by2(upper, lower_hi, div);
    auto [quotient, remainder] = divide_3by2(remainder_hi.digits, lower, div);
    quotient += quotient_hi.left_shift(n);
    return pair(move(quotient), move(remainder));
  }

  pair<int_base10, int_base10> divide_3by2(
    vector<int>& upper, vector<int>& lower, const int_base10& div) const {
    size_t n = div.digits.size() / 2;
    int_base10 div_hi(vector(div.digits.begin() + n, div.digits.end()));
    assert(div_hi.digits.size() == n);
    int_base10 div_lo(vector(div.digits.begin(), div.digits.begin() + n));
    int_base10 quotient, remainder;
    bool less_than = (upper.size() < 2 * n);
    if (!less_than) {
      for (size_t i = 1; i <= n; i++) {
        if (upper[2 * n - i] != div_hi[n - i]) {
          less_than = (upper[2 * n - i] < div_hi[n - i]);
          break;
        }
      }
    }
    if (less_than) {
      vector<int> upper_hi((upper.size() <= n ? upper.end() : upper.begin() + n), upper.end());
      upper.resize(n);
      tie(quotient, remainder) = divide_2by1(upper_hi, upper, div_hi);
    } else {
      quotient = int_base10(vector<int>(n, radix - 1));
      remainder = move(upper);
      remainder += div_hi;
      remainder -= div_hi.left_shift(n);
      assert(!remainder.negative);
    }
    remainder.left_shift(n);
    remainder += int_base10(move(lower));
    remainder -= quotient * div_lo;
    while (remainder.negative) {
      remainder += div;
      quotient -= 1;
    }
    return pair(quotient, remainder);
  }

  int_base10& fix_leading_zeros() {
    if (digits.empty()) {
      digits.push_back(0);
    } else {
      while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
      }
    }
    return *this;
  }
  int_base10& left_shift(size_t shift) {
    digits.insert(digits.begin(), shift, 0);
    return *this;
  }
  int_base10& right_shift(size_t shift) {
    if (digits.size() <= shift) {
      return clear();
    }
    digits.erase(digits.begin(), digits.begin() + shift);
    return *this;
  }
};

