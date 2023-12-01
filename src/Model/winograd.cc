#include "winograd.h"

int main() {
  std::vector<std::vector<double>> one{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  std::vector<std::vector<double>> two{{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
  Winograd w(one, two);
  auto row_f = w.RowFactor();
  auto col_f = w.ColFactor();
  auto res = w.WinogradMulti(row_f, col_f);

  if (one.size() % 2 != 0) w.IfNotEven(res);

  for (auto &r : res) {
    for (auto &print : r) {
      std::cout << print << " ";
    }
    std::cout << std::endl;
  }
  return 0;
}