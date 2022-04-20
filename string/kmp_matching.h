/* KMP String Matching
 * USAGE
 *  kmp_matching kmp(pattern);
 *  kmp.match(s);
 * TIME
 *  O(|s|)
 * STATUS
 *  untested: kattis/stringmatching
 */
#pragma once

#include <string>
#include <vector>

template <typename T>
struct kmp_matching {
  std::basic_string<T> pattern;
  std::vector<int> fail;
  kmp_matching(const std::basic_string<T>& s): pattern(s), fail(pattern.size(), -1) { build(); }
  kmp_matching(std::basic_string<T>&& s): pattern(move(s)), fail(pattern.size(), -1) { build(); }

  std::vector<int> find_all(const std::basic_string<T>& s) {
    std::vector<int> matches;
    for (int i = 0, state = -1; i < (int)s.size(); i++) {
      while (state != -1 && pattern[state + 1] != s[i]) {
        state = fail[state];
      }
      state += (pattern[state + 1] == s[i]);
      if (state + 1 == (int)pattern.size()) {
        matches.push_back(i + 1 - (int)pattern.size());
        state = fail[state];
      }
    }
    return matches;
  }

  int find_first(const std::basic_string<T>& s) {
    for (int i = 0, state = -1; i < (int)s.size(); i++) {
      while (state != -1 && pattern[state + 1] != s[i]) {
        state = fail[state];
      }
      state += (pattern[state + 1] == s[i]);
      if (state + 1 == (int)pattern.size()) {
        return i + 1 - (int)pattern.size();
      }
    }
    return s.size();
  }

private:
  void build() {
    for (int i = 0, state = -1; i < (int)pattern.size(); i++) {
      while (state != -1 && pattern[i] != pattern[state + 1]) {
        state = fail[state];
      }
      state += (state + 1 < i && pattern[i] == pattern[state + 1]);
      fail[i] = state;
    }
  }
};

