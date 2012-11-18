#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>

#include "list.hpp"

using namespace org_bykn_istanbul;

int main() {
  auto v = List<int>::empty();
  v = List<int>::cons(1,v);
  v = List<int>::cons(2,v);
  // STL foreach:
  std::for_each(v->begin(), v->end(), [](int x) { std::cout << "stl: " << x << std::endl; });
  std::cout << "done" << std::endl << std::flush;
  std::cout << "equals: " << (*v == *v) << std::endl;
  std::cout << "equals tail: " << (*v == *(v->tail())) << std::endl;
  std::cout << "equals rev: " << (*v == *(v->reverse()->reverse())) << std::endl;
  // our foreach:
  v->foreach([](int x) { std::cout << x; });

  v->reverse()
    ->foreach([](int x) { std::cout << x; });

  v->map<int>([](int x) { return x*x; })
    ->foreach([](int x) { std::cout << x; });

  std::cout << ((*v)[0]) << ((*v)[1]);
  std::cout << std::flush;
}
