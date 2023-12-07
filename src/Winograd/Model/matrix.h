#ifndef SRC_MODEL_MATRIX_H_
#define SRC_MODEL_MATRIX_H_

#include <iostream>
#include <vector>

namespace s21 {
class Matrix {
 public:
  void SetRows(int rows) { rows_ = rows; }
  void SetCols(int cols) { cols_ = cols; }

  int GetRows() { return rows_; }
  int GetCols() { return cols_; }

  double& operator()(int row, int columns) {
    return matrix_[row * cols_ + columns];
  }

  void Clear() { matrix_.clear(); }

  void Reserve() { matrix_.reserve(rows_ * cols_); }

  void Push(double num) { matrix_.emplace_back(num); }

  void Print() {
    int c = 1;
    for (int i = 0; i < rows_ * cols_; ++i) {
      std::cout << matrix_[i] << " ";
      if (c % cols_ == 0) std::cout << std::endl;
      c++;
    }
  }

 private:
  std::vector<double> matrix_;
  int rows_ = 0;
  int cols_ = 0;
};
}  // namespace s21

#endif  //  SRC_MODEL_MATRIX_H_
