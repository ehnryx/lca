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

#include <array>
#include <string>
#include <vector>

template <template<typename> typename container_t, typename T>
struct kmp_matching {
  container_t<T> pattern;
  std::vector<int> fail;
  kmp_matching(const container_t<T>& s): pattern(s), fail(pattern.size(), -1) { build(); }
  kmp_matching(container_t<T>&& s): pattern(move(s)), fail(pattern.size(), -1) { build(); }

  template <template<typename> typename container_u>
  std::vector<int> find_all(const container_u<T>& s) {
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

  template <template<typename> typename container_u>
  int find_first(const container_u<T>& s) {
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

  // dfa nodes correspond to the 1-indexed string
  template <typename to_int_t>
  std::vector<std::array<int, to_int_t::size>> build_dfa() const {
    std::vector<std::array<int, to_int_t::size>> dfa(1);
    if (pattern.empty()) {
      return dfa;
    }
    for (int i = 0; i < (int)pattern.size(); i++) {
      dfa.back()[to_int_t()(pattern[i])] = i + 1;
      dfa.emplace_back(dfa[fail[i] + 1]);
    }
    return dfa;
  }

private:
  void build() {
    for (int i = 1, state = -1; i < (int)pattern.size(); i++) {
      while (state != -1 && pattern[i] != pattern[state + 1]) {
        state = fail[state];
      }
      state += (pattern[i] == pattern[state + 1]);
      fail[i] = state;
    }
  }
};

