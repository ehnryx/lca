#!/bin/bash

temp="temp_unused_filename"
echo "#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
const ld EPS = 1e-9; // for geometry
int main() {}" > $temp.cpp

echo "test compile"
for arg; do
  echo "  $arg"
  echo "#include \"$arg\"" >> $temp.cpp
done

g++ -Wall -Wextra -Wshadow -Wfatal-errors -Wpedantic -Wconversion \
  -std=c++17 $temp.cpp -o $temp && rm $temp
rm $temp.cpp
