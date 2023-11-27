#ifndef A2_SIMPLENAVIGATOR_SRC_MODEL_ADJ_MATRIX_H_
#define A2_SIMPLENAVIGATOR_SRC_MODEL_ADJ_MATRIX_H_

#include <cmath>
#include <iostream>
#include <vector>

namespace s21 {

class AdjMatrix {
 public:
  AdjMatrix() = default;

  void Init(int size);

  void InitWithNumber(int size, double num);

  void Push(double num);

  void Print();

  void Clear();

  double& operator()(int row, int columns);

  const double& operator()(int row, int columns) const;

  std::vector<double>& GetData();

  const std::vector<double>& GetData() const;

 private:
  int offset_ = 0;
  std::vector<double> data_;
};
}  // namespace s21

#endif  //  A2_SIMPLENAVIGATOR_SRC_MODEL_ADJ_MATRIX_H_