#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <mutex>
#include <vector>

int main() {
  std::locale::global(std::locale("C"));
  std::vector<double> v(100000000, 21.5);
  double sum_1 = 0;
  std::mutex m;
  auto t1 = std::chrono::high_resolution_clock::now();
  std::for_each(std::execution::par, std::begin(v), std::end(v),
                [&sum_1, &m](int i) {
                  std::lock_guard<std::mutex> lock(m);
                  sum_1 += i;
                });
  auto t2 = std::chrono::high_resolution_clock::now();
  auto total =
      std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  std::cout << sum_1 << " - " << total << std::endl;
  double sum_2 = 0;
  auto t3 = std::chrono::high_resolution_clock::now();
  std::for_each(std::begin(v), std::end(v), [&](int i) { sum_2 += i; });
  auto t4 = std::chrono::high_resolution_clock::now();
  total =
      std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
  std::cout << sum_2 << " - " << total << std::endl;
  return 0;
}
