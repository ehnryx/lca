%:include "utility/overload.h"

#include <functional>
#include <iostream>
#include <string>

void g(int, int) { std::cout << "g works too" << std::endl; }

int main() {
  int x = 7;
  auto f = overload{
    //[](int) { std::cout << "int" << std::endl; },
    //[](double) { std::cout << "double" << std::endl; },
    //[]() { std::cout << "void" << std::endl; },
    [](int) { std::cout << "int " << std::endl; },
    [](double) { std::cout << "double " << std::endl; },
    [&]() { std::cout << "void " << x << std::endl; },
    [x](std::string) { std::cout << "std::string " << x << std::endl; },
    std::function(g),
  };
  f();
  f(0);
  f(0.);
  f(0, 0);
  f("ehh"); // not a string
  return 0;
}
