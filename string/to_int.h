/* stuff
 * USAGE
 *  use it
 * TIME
 *  constant
 * STATUS
 *  untested
 */
#pragma once

namespace to_int {
struct lower {
  int operator()(const char c) const { return c - 'a'; }
  static constexpr int size = 26;
};

struct upper {
  int operator()(const char c) const { return c - 'A'; }
  static constexpr int size = 26;
};

struct alpha {
  int operator()(const char c) const { return c < 'a' ? c - 'A' : c - 'a' + 26; }
  static constexpr int size = 52;
};

struct digits {
  int operator()(const char c) const { return c - '0'; }
  static constexpr int size = 10;
};

struct lower_digits {
  int operator()(const char c) const { return c < 'a' ? c - '0' : c - 'a' + 10; }
  static constexpr int size = 36;
};

struct upper_digits {
  int operator()(const char c) const { return c < 'A' ? c - '0' : c - 'A' + 10; }
  static constexpr int size = 36;
};

struct alpha_digits {
  int operator()(const char c) const {
    return c < 'A' ? c - '0' : c < 'a' ? c - 'A' + 10 : c - 'a' + 36;
  }
  static constexpr int size = 62;
};
}  // namespace to_int
