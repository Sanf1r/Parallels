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
  Model() = default;

  bool LoadMatrix(const std::string& f_path, const std::string& s_path);

  void BeforeCalculation(int thread_num);

  void Standart(int loops);
  void Parallel(int loops);
  void Pipeline(int loops);

 private:
  std::vector<std::vector<double>> in_1_;
  std::vector<std::vector<double>> in_2_;
  int row_one = 0;
  int col_one = 0;
  int row_two = 0;
  int col_two = 0;
  int half_ = 0;
  bool even_ = false;

  std::vector<std::vector<double>> standart_result_;
  std::vector<std::vector<double>> parallel_result_;
  std::vector<std::vector<double>> pipeline_result_;

  bool LoadLogicFirst(const std::string& f_path,
                      std::vector<std::vector<double>>& matrix);
  bool LoadLogicSecond(const std::string& s_path,
                       std::vector<std::vector<double>>& matrix);

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
