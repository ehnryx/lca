#!/bin/bash

temp="temp_unused_filename"
echo "#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
const ld EPS = 1e-9; // for geometry
int main() {}" > $temp.cpp

echo "test compile"
cpp_files=()
for arg; do
  if [[ $arg = *.h ]]; then
    echo "  $arg"
    echo "#include \"$arg\"" >> $temp.cpp
  else
    cpp_files+=($arg)
  fi
done

echo "compiling headers"
g++-11 -Wall -Wextra -Wshadow -Wfatal-errors -Wpedantic -Wconversion \
  -D HENRYX -std=c++20 $temp.cpp -o $temp && rm $temp
rm $temp.cpp

for f in ${cpp_files[@]}; do
  printf "\ncompiling $f\n"
  g++-11 -Wall -Wextra -Wshadow -Wfatal-errors -Wpedantic -Wconversion \
    -D HENRYX -std=c++20 $f -o $temp && rm $temp
done

