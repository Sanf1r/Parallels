#ifndef SRC_MODEL_WINOGRAD_H_
#define SRC_MODEL_WINOGRAD_H_

#include <omp.h>

#include <iostream>
#include <queue>
#include <thread>
#include <vector>

class Winograd {
 public:
  Winograd(std::vector<std::vector<double>>& in_1,
           std::vector<std::vector<double>>& in_2)
      : in_1_(in_1), in_2_(in_2) {
    row_f_size_ = in_1_.size();
    col_f_size_ = in_2_[0].size();
    half_ = row_f_size_ / 2;
  }

  std::vector<double> RowFactor() {
    std::vector<double> result(row_f_size_);
    for (int row = 0; row < row_f_size_; ++row) {
      for (int j = 0; j < half_; ++j)
        result[row] += in_1_[row][2 * j] * in_1_[row][2 * j + 1];
    }
    return result;
  }

  std::vector<double> ColFactor() {
    std::vector<double> result(col_f_size_);
    for (int col = 0; col < col_f_size_; ++col) {
      for (int j = 0; j < half_; ++j)
        result[col] += in_2_[2 * j][col] * in_2_[2 * j + 1][col];
    }
    return result;
  }

  std::vector<std::vector<double>> WinogradMulti(std::vector<double>& row_f,
                                                 std::vector<double>& col_f) {
    std::vector<std::vector<double>> result(row_f_size_);
    for (auto& row : result) row.resize(col_f_size_);

    for (int row = 0; row < row_f_size_; ++row) {
      for (int col = 0; col < col_f_size_; ++col) {
        result[row][col] = -row_f[row] - col_f[col];
        for (auto k = 0; k < half_; ++k) {
          result[row][col] += (in_1_[row][2 * k] + in_2_[2 * k + 1][col]) *
                              (in_1_[row][2 * k + 1] + in_2_[2 * k][col]);
        }
      }
    }

    return result;
  }

  void IfNotEven(std::vector<std::vector<double>>& result) {
    for (int row = 0; row < row_f_size_; ++row)
      for (int col = 0; col < col_f_size_; ++col)
        result[row][col] +=
            in_1_[row][col_f_size_ - 1] * in_2_[row_f_size_ - 1][col];
  }

  std::vector<std::vector<double>> Pipeline() {
    std::vector<std::vector<double>> result_matrix(row_f_size_);
    for (auto& row : result_matrix) row.resize(col_f_size_);

    std::queue<double> row_q;
    // std::queue<double> col_q;

    auto col_factor = ColFactor();
    std::cout << "PIPELINE" << std::endl;
    std::thread t1([&]() {
      for (int row = 0; row < row_f_size_; ++row) {
        double result = 0;
        for (int j = 0; j < half_; ++j) {
          result += in_1_[row][2 * j] * in_1_[row][2 * j + 1];
        }
        row_q.emplace(result);
      }
    });

    // std::thread t2([&]() {
    //   for (int col = 0; col < col_f_size_; ++col) {
    //     double result = 0;
    //     for (int j = 0; j < half_; ++j) {
    //       result += in_2_[2 * j][col] * in_2_[2 * j + 1][col];
    //     }
    //     col_q.emplace(result);
    //   }
    // });

    int stop = row_f_size_ * col_f_size_;

    std::thread t3([&]() {
      std::cout << "T3" << std::endl;
      int c = 0, ro = 0;
      while (c != stop) {
        if (!row_q.empty()) {
          for (int col = 0; col < col_f_size_; ++col) {
            result_matrix[ro][col] = -row_q.front() - col_factor[col];
            c++;
            for (auto k = 0; k < half_; ++k) {
              result_matrix[ro][col] +=
                  (in_1_[ro][2 * k] + in_2_[2 * k + 1][col]) *
                  (in_1_[ro][2 * k + 1] + in_2_[2 * k][col]);
            }
          }

          ro++;
          row_q.pop();
          //   col_q.pop();
          std::cout << "c = " << c << std::endl;
        }
      }
    });

    t1.join();
    std::cout << "T1 done" << std::endl;
    // t2.join();
    std::cout << "t2 done" << std::endl;
    t3.join();

    return result_matrix;
  }

 private:
  std::vector<std::vector<double>> in_1_;
  std::vector<std::vector<double>> in_2_;
  int row_f_size_ = 0;
  int col_f_size_ = 0;
  int half_ = 0;

  //   std::queue<double> row_q;
  //   std::queue<double> col_q;
};

#endif  //  SRC_MODEL_WINOGRAD_H_
