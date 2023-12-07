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

#include "tsqueue.h"

namespace s21 {

class Model {
 public:
  Model(std::vector<std::vector<double>>& in_1,
        std::vector<std::vector<double>>& in_2);

  std::vector<std::vector<double>> Standart();

  std::vector<std::vector<double>> Parallel();

  std::vector<std::vector<double>> Pipeline();

 private:
  std::vector<std::vector<double>> in_1_;
  std::vector<std::vector<double>> in_2_;
  int row_one = 0;
  int col_one = 0;
  int row_two = 0;
  int col_two = 0;
  int half_ = 0;
  bool even_ = false;

  std::vector<double> RowFactor();
  std::vector<double> ColFactor();
  std::vector<std::vector<double>> WinogradMulti(std::vector<double>& row_f,
                                                 std::vector<double>& col_f);
  void IfNotEven(std::vector<std::vector<double>>& result);

  std::vector<double> RowFactorParallel();
  std::vector<double> ColFactorParallel();
  std::vector<std::vector<double>> WinogradMultiParallel(
      std::vector<double>& row_f, std::vector<double>& col_f);
  void IfNotEvenParallel(std::vector<std::vector<double>>& result);
};

}  // namespace s21

#endif  //  SRC_MODEL_WINOGRAD_H_
