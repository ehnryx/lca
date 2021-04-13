#include <bits/stdc++.h>
using namespace std;

////////////////////////////////////////////////////////////////////////
// Z Value String Matching -- O(n+m)
// n = length of string; m = length of pattern
// USAGE:
//  0. given string s and pattern t
//  1. vector<int> z = zValue(t+s);
//  2. for i from m to m+n-1: match at i-m if z[i] >= m;
//*!
vector<int> zValue(const string& s) {
    int n = s.size();
    vector<int> z(n, n);
    for (int i=1,l=0,r=0; i<n; i++) {
        int e = max(i, min(i+z[i-l], r));
        while (e<n && s[e]==s[e-i]) e++;
        if ((z[i]=e-i)>r-i) l=i,r=e;
    }
    return z;
}
//*/

////////////////////////////////////////////////////////////////////////
// KMP String Matching (prefix function) -- O(n+m)
// n = length of string; m = length of pattern
// USAGE:
//  0. given string s and pattern t
//  1. vector<int> matches = kmp(s, t);
//*!
vector<int> prefixFunc(const string& s) {
    int n = s.size();
    vector<int> p(n, 0);
    for (int i=1,j=0; i<n; i++) {
        while (j>0 && s[i]!=s[j]) j = p[j-1];
        p[i] = (s[i]==s[j] ? ++j : j);
    }
    return p;
}
vector<int> kmp(const string& s, const string& t) {
    vector<int> p = prefixFunc(t);
    vector<int> res;
    int n = s.size();
    int m = t.size();
    for (int i=0,j=0; i<n; i++) {
        while (j>0 && s[i]!=t[j]) j = p[j-1];
        if ((s[i]==t[j] ? ++j : j) == m) {
            res.push_back(i-m+1); // store match
            j = p[j-1];
        }
    }
    return res;
}
//*/

////////////////////////////////////////////////////////////////////////
// Aho-Corasick String Matching -- O(n + m + numMatches)
// WARNING: all pattern strings must be different; patterns 0-indexed
//          change `int id` to `vector<int> id` for non-unique patterns
// USAGE:
//  0. AhoCorasick trie;
//  1. trie.insert(pattern) to insert patterns
//  2. trie.build() to build links once all words are inserted
//  3. auto matches = trie.match(string) to find words in string
//*!
struct AhoCorasick {
    struct Node {
        Node *end, *p;
        unordered_map<char,Node*> ch;
        int id, d;
        Node(int d=0): id(-1), d(d) {}
    };

    Node* root;
    int wcnt;
    AhoCorasick(): root(new Node), wcnt(0) {}

    void insert(const string& s) {
        Node* u = root;
        for (char c : s) {
            if (!u->ch.count(c)) u->ch[c] = new Node(u->d+1);
            u = u->ch[c];
        }
        assert(u->id == -1); u->id = wcnt++;
    }

    void build() {
        root->end = root; root->p = 0;
        queue<Node*> bfs; bfs.push(root);
        while (!bfs.empty()) {
            Node* u = bfs.front(); bfs.pop();
            for (const auto& it : u->ch) {
                char c; Node* v; tie(c,v) = it;
                Node* j = u->p;
                while (j && !j->ch.count(c)) j = j->p;
                v->p = (j ? j->ch[c] : root);
                v->end = (v->id == -1 ? v->p->end : v);
                bfs.push(v);
            }
        }
    }

    vector<vector<int>> match(const string& s) {
        vector<vector<int>> m(wcnt, vector<int>());
        Node* u = root;
        for (int i=0; i<s.size(); i++) {
            while (u!=root && !u->ch.count(s[i])) u = u->p;
            if (u->ch.count(s[i])) u = u->ch[s[i]];
            for (Node* v=u; v->end!=root; v=v->p) {
                v = v->end; // get matches
                m[v->id].push_back(i-v->d+1);
            }
        }
        return m;
    }
};
//*/

////////////////////////////////////////////////////////////////////////
int main() {
    return 0;
}
