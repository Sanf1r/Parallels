#ifndef PARALLELS_SRC_WINOGRAD_MODEL_MODEL_H_
#define PARALLELS_SRC_WINOGRAD_MODEL_MODEL_H_

#include <omp.h>

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ostream>
#include <queue>
#include <random>
#include <sstream>
#include <thread>
#include <vector>

#include "matrix.h"
#include "tsqueue.h"

namespace s21 {

class Model {
 public:
  Model() = default;

  bool LoadMatrix(const std::string& f_path, const std::string& s_path);
  void GenerateMatrix(int f_rows, int f_cols, int s_rows, int s_cols);

  bool BeforeCalculation(int thread_num);

  void Standart(int loops);
  void Parallel(int loops);
  void Pipeline(int loops);

  void PrintStandartRes() { standart_result_.Print(); }
  void PrintParallelRes() { parallel_result_.Print(); }
  void PrintPipelineRes() { pipeline_result_.Print(); }

 private:
  Matrix in_1_;
  Matrix in_2_;
  int row_one_ = 0;
  int col_one_ = 0;
  int row_two_ = 0;
  int col_two_ = 0;
  int half_ = 0;
  bool even_ = false;

  Matrix standart_result_;
  Matrix parallel_result_;
  Matrix pipeline_result_;

  bool LoadLogic(const std::string& path, Matrix& matrix);
  void GenerateLogic(int rows, int cols, Matrix& matrix);

  std::vector<double> RowFactor();
  std::vector<double> ColFactor();
  void WinogradMulti(const std::vector<double>& row_f,
                     const std::vector<double>& col_f);
  void IfNotEven();

  std::vector<double> RowFactorParallel();
  std::vector<double> ColFactorParallel();
  void WinogradMultiParallel(const std::vector<double>& row_f,
                             const std::vector<double>& col_f);
  void IfNotEvenParallel();
};

}  // namespace s21

#endif  //  PARALLELS_SRC_WINOGRAD_MODEL_MODEL_H_
