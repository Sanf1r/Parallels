#ifndef SRC_MODEL_WINOGRAD_H_
#define SRC_MODEL_WINOGRAD_H_

#include <omp.h>

#include <iostream>
#include <vector>

class Winograd {
 public:
  Winograd(std::vector<std::vector<double>>& in_1,
           std::vector<std::vector<double>>& in_2)
      : in_1_(in_1), in_2_(in_2) {}

  std::vector<double> RowFactor() {
    auto size = in_1_.size();
    auto half_size = size / 2;
    std::vector<double> result(size);
    for (auto row = 0u; row < size; ++row) {
      for (auto j = 0u; j < half_size; ++j)
        result[row] += in_1_[row][2 * j] * in_1_[row][2 * j + 1];
    }
    return result;
  }

  std::vector<double> ColFactor() {
    auto size = in_2_[0].size();
    auto half_size = size / 2;
    std::vector<double> result(size);

    for (auto col = 0u; col < size; ++col) {
      for (auto j = 0u; j < half_size; ++j)
        result[col] += in_2_[2 * j][col] * in_2_[2 * j + 1][col];
    }
    return result;
  }

  std::vector<std::vector<double>> WinogradMulti(std::vector<double>& row_f,
                                                 std::vector<double>& col_f) {
    auto rows = in_1_.size();
    auto half_rows = rows / 2;
    auto cols = in_2_[0].size();

    std::vector<std::vector<double>> result(rows);
    for (auto& row : result) row.resize(cols);

    for (auto row = 0u; row < rows; ++row) {
      for (auto col = 0u; col < cols; ++col) {
        result[row][col] = -row_f[row] - col_f[col];
        for (auto k = 0u; k < half_rows; ++k) {
          result[row][col] += (in_1_[row][2 * k] + in_2_[2 * k + 1][col]) *
                              (in_1_[row][2 * k + 1] + in_2_[2 * k][col]);
        }
      }
    }

    return result;
  }

  void IfNotEven(std::vector<std::vector<double>>& result) {
    auto rows = in_1_.size();
    auto cols = in_2_[0].size();

    for (auto row = 0u; row < rows; ++row)
      for (auto col = 0u; col < cols; ++col)
        result[row][col] += in_1_[row][cols - 1] * in_2_[rows - 1][col];
  }

 private:
  std::vector<std::vector<double>> in_1_;
  std::vector<std::vector<double>> in_2_;
};

#endif  //  SRC_MODEL_WINOGRAD_H_
