#include <algorithm>
#include <execution>
#include <iostream>
#include <vector>

int main() {
  std::vector<int> v = {1, 2, 3, 4, 5};
  int sum = 0;
  std::for_each(std::execution::par, std::begin(v), std::end(v),
                [&](int i) { sum += i; });
  std::cout << sum << std::endl;
  return 0;
}