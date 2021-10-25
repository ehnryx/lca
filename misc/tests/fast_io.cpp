#include <bits/stdc++.h>
using namespace std;

#include "../fast_input.h"

constexpr char nl = '\n';

fast_input<1000> in;
//fast_output out;
#define out cout

int main() {
  cin.tie(0)->sync_with_stdio(0);

  char v_char = -1;
  int v_int = -1;
  long long v_long_long = -1;
  float v_float = -1;
  double v_double = -1;
  long double v_long_double = -1;
  string v_string = "-1";

  string s;
  while (in.read(s), s != "done") {
    if(s == "char") {
      in >> v_char; //in.read(v_char);
      out << "read char: " << v_char << nl;
    } else if(s == "int") {
      in >> v_int; //in.read(v_int);
      out << "read int: " << v_int << nl;
    } else if(s == "long_long") {
      in >> v_long_long; //in.read(v_long_long);
      out << "read long_long: " << v_long_long << nl;
    } else if(s == "float") {
      in >> v_float; //in.read(v_float);
      out << "read float: " << v_float << nl;
    } else if(s == "double") {
      in >> v_double; //in.read(v_double);
      out << "read double: " << v_double << nl;
    } else if(s == "long_double") {
      in >> v_long_double; //in.read(v_long_double);
      out << "read long_double: " << v_long_double << nl;
    } else if(s == "string") {
      in >> v_string; //in.read(v_string);
      out << "read string: " << v_string << nl;
    }
  }

  return 0;
}
