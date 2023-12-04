#ifndef SRC_MODEL_WINOGRAD_H_
#define SRC_MODEL_WINOGRAD_H_

#include <omp.h>

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ostream>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>

class Winograd {
 public:
  Winograd(std::vector<std::vector<double>>& in_1,
           std::vector<std::vector<double>>& in_2)
      : in_1_(in_1), in_2_(in_2) {
    row_one = in_1_.size();
    col_one = in_1_[0].size();
    row_two = in_2_.size();
    col_two = in_2_[0].size();
    if (row_two % 2 == 0) even_ = true;
    half_ = row_two / 2;
  }

  std::vector<std::vector<double>> Standart() {
    auto row_f = RowFactor();

    auto col_f = ColFactor();

    auto res = WinogradMulti(row_f, col_f);

    if (!even_) IfNotEven(res);

    return res;
  }

  std::vector<double> RowFactor() {
    std::vector<double> result(row_one);
    for (int row = 0; row < row_one; ++row) {
      for (int j = 0; j < half_; ++j)
        result[row] += in_1_[row][2 * j] * in_1_[row][2 * j + 1];
    }
    return result;
  }

  std::vector<double> ColFactor() {
    std::vector<double> result(col_two);
    for (int col = 0; col < col_two; ++col) {
      for (int j = 0; j < half_; ++j)
        result[col] += in_2_[2 * j][col] * in_2_[2 * j + 1][col];
    }
    return result;
  }

  std::vector<double> ColFactorParallel() {
    std::vector<double> result(col_two);
#pragma omp parallel for schedule(dynamic)
    for (int col = 0; col < col_two; ++col) {
      for (int j = 0; j < half_; ++j)
        result[col] += in_2_[2 * j][col] * in_2_[2 * j + 1][col];
    }
    return result;
  }

  std::vector<std::vector<double>> WinogradMulti(std::vector<double>& row_f,
                                                 std::vector<double>& col_f) {
    std::vector<std::vector<double>> result(row_one);
    for (auto& row : result) row.resize(col_two);

    for (int row = 0; row < row_one; ++row) {
      for (int col = 0; col < col_two; ++col) {
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
    for (int row = 0; row < row_one; ++row)
      for (int col = 0; col < col_two; ++col)
        result[row][col] += in_1_[row][row_two - 1] * in_2_[row_two - 1][col];
  }

  // std::vector<std::vector<double>> Pipeline() {
  //   std::vector<std::vector<double>> result_matrix(row_one);
  //   for (auto& row : result_matrix) row.resize(col_two);

  //   std::queue<double> row_q;
  //   std::queue<double> mult_q;

  //   bool t1_done = false;
  //   bool t2_done = false;
  //   bool t3_done = false;

  //   auto col_factor = ColFactorParallel();

  //   std::thread t1([&]() {
  //     std::unique_lock<std::mutex> lock(gLock);

  //     for (int row = 0; row < row_one; ++row) {
  //       double result = 0;
  //       for (int j = 0; j < half_; ++j) {
  //         result += in_1_[row][2 * j] * in_1_[row][2 * j + 1];
  //       }
  //       row_q.emplace(result);
  //       t1_done = true;
  //       gConVar.notify_one();
  //     }
  //   });

  //   int stop = row_one * col_two;

  //   std::thread t2([&]() {
  //     std::unique_lock<std::mutex> lock(gLock);
  //     int c = 0, col = 0;
  //     while (c != stop) {
  //       while (!t1_done) {
  //         gConVar.wait(lock);
  //       }
  //       mult_q.emplace(-row_q.front() - col_factor[col++]);
  //       t2_done = true;
  //       gConVar_2.notify_one();
  //       c++;
  //       if (c % col_two == 0) {
  //         row_q.pop();
  //         col = 0;
  //       }
  //     }
  //   });

  //   std::thread t3([&]() {
  //     std::unique_lock<std::mutex> lock(gLock);
  //     int c = 0, ro = 0, co = 0;
  //     while (c != stop) {
  //       while (!t2_done) {
  //         gConVar_2.wait(lock);
  //       }
  //       result_matrix[ro][co] = mult_q.front();
  //       for (auto k = 0; k < half_; ++k) {
  //         result_matrix[ro][co] += (in_1_[ro][2 * k] + in_2_[2 * k + 1][co])
  //         *
  //                                  (in_1_[ro][2 * k + 1] + in_2_[2 * k][co]);
  //       }
  //       t3_done = true;
  //       gConVar_3.notify_one();
  //       co++;
  //       if (co == col_two) {
  //         co = 0;
  //         ro++;
  //       };
  //       c++;
  //       mult_q.pop();
  //     }
  //   });

  //   std::thread t4([&]() {
  //     std::unique_lock<std::mutex> lock(gLock);
  //     int c = 0, ro = 0, co = 0;
  //     if (even_) c = stop;
  //     while (c != stop) {
  //       while (!t3_done) {
  //         gConVar_3.wait(lock);
  //       }

  //       result_matrix[ro][co] +=
  //           in_1_[ro][row_two - 1] * in_2_[row_two - 1][co];
  //       co++;

  //       if (co == col_two) {
  //         ro++;
  //         co = 0;
  //       }
  //       c++;
  //     }
  //   });

  //   if (t1.joinable()) t1.join();
  //   if (t2.joinable()) t2.join();
  //   if (t3.joinable()) t3.join();
  //   if (t4.joinable()) t4.join();
  //   return result_matrix;
  // }

  std::vector<std::vector<double>> Pipeline() {
    std::vector<std::vector<double>> result_matrix(row_one);
    for (auto& row : result_matrix) row.resize(col_two);

    std::queue<double> row_q;
    std::queue<double> mult_q;

    bool t1_done = false;
    bool t2_done = false;
    bool t3_done = false;

    auto col_factor = ColFactorParallel();

    std::thread t1([&]() {
      std::unique_lock<std::mutex> lock(gLock);

      for (int row = 0; row < row_one; ++row) {
        double result = 0;
        for (int j = 0; j < half_; ++j) {
          result += in_1_[row][2 * j] * in_1_[row][2 * j + 1];
        }
        row_q.emplace(result);
        t1_done = true;
        gConVar.notify_one();
      }
    });

    int stop = row_one * col_two;

    std::thread t2([&]() {
      std::unique_lock<std::mutex> lock(gLock);
      int c = 0, col = 0;
      while (c != stop) {
        while (!t1_done) {
          gConVar.wait(lock);
        }
        mult_q.emplace(-row_q.front() - col_factor[col++]);
        t2_done = true;
        gConVar_2.notify_one();
        c++;
        if (c % col_two == 0) {
          row_q.pop();
          col = 0;
        }
      }
    });

    std::thread t3([&]() {
      std::unique_lock<std::mutex> lock(gLock);
      int c = 0, ro = 0, co = 0;
      while (c != stop) {
        while (!t2_done) {
          gConVar_2.wait(lock);
        }
        result_matrix[ro][co] = mult_q.front();
        for (auto k = 0; k < half_; ++k) {
          result_matrix[ro][co] += (in_1_[ro][2 * k] + in_2_[2 * k + 1][co]) *
                                   (in_1_[ro][2 * k + 1] + in_2_[2 * k][co]);
        }
        t3_done = true;
        gConVar_3.notify_one();
        co++;
        if (co == col_two) {
          co = 0;
          ro++;
        };
        c++;
        mult_q.pop();
      }
    });

    std::thread t4([&]() {
      std::unique_lock<std::mutex> lock(gLock);
      int c = 0, ro = 0, co = 0;
      if (even_) c = stop;
      while (c != stop) {
        while (!t3_done) {
          gConVar_3.wait(lock);
        }

        result_matrix[ro][co] +=
            in_1_[ro][row_two - 1] * in_2_[row_two - 1][co];
        co++;

        if (co == col_two) {
          ro++;
          co = 0;
        }
        c++;
      }
    });

    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();
    if (t3.joinable()) t3.join();
    if (t4.joinable()) t4.join();
    return result_matrix;
  }

 private:
  std::vector<std::vector<double>> in_1_;
  std::vector<std::vector<double>> in_2_;
  int row_one = 0;
  int col_one = 0;
  int row_two = 0;
  int col_two = 0;
  int half_ = 0;
  bool even_ = false;

  std::mutex gLock;
  std::condition_variable gConVar;
  std::condition_variable gConVar_2;
  std::condition_variable gConVar_3;
};

#endif  //  SRC_MODEL_WINOGRAD_H_
