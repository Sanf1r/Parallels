#include "adj_matrix.h"

namespace s21 {

void AdjMatrix::Init(int size) {
  data_.reserve(size * size);
  offset_ = size;
}

void AdjMatrix::InitWithNumber(int size, double num) {
  data_.resize(size * size, num);
  offset_ = size;
}

void AdjMatrix::Push(double num) { data_.push_back(num); }

void AdjMatrix::Print() {
  int c = 1;
  for (int i = 0; i < (int)data_.size(); ++i) {
    if (data_[i] == INFINITY) {
      std::cout << "0 ";
    } else {
      std::cout << data_[i] << " ";
    }

    if (c % offset_ == 0) std::cout << std::endl;
    c++;
  }
}

void AdjMatrix::Clear() { data_.clear(); }

double& AdjMatrix::operator()(int row, int columns) {
  return data_[row * offset_ + columns];
}

const double& AdjMatrix::operator()(int row, int columns) const {
  return data_[row * offset_ + columns];
}

std::vector<double>& AdjMatrix::GetData() { return data_; }

const std::vector<double>& AdjMatrix::GetData() const { return data_; }

}  // namespace s21
