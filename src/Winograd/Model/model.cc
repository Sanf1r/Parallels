#include "model.h"

namespace s21 {

Model::Model(std::vector<std::vector<double>>& in_1,
             std::vector<std::vector<double>>& in_2)
    : in_1_(in_1), in_2_(in_2) {
  row_one = in_1_.size();
  col_one = in_1_[0].size();
  row_two = in_2_.size();
  col_two = in_2_[0].size();
  if (row_two % 2 == 0) even_ = true;
  half_ = row_two / 2;
}

std::vector<std::vector<double>> Model::Standart() {
  auto row_f = RowFactor();

  auto col_f = ColFactor();

  auto res = WinogradMulti(row_f, col_f);

  if (!even_) IfNotEven(res);

  return res;
}

std::vector<double> Model::RowFactor() {
  std::vector<double> result(row_one);
  for (int row = 0; row < row_one; ++row) {
    for (int j = 0; j < half_; ++j)
      result[row] += in_1_[row][2 * j] * in_1_[row][2 * j + 1];
  }
  return result;
}

std::vector<double> Model::ColFactor() {
  std::vector<double> result(col_two);
  for (int col = 0; col < col_two; ++col) {
    for (int j = 0; j < half_; ++j)
      result[col] += in_2_[2 * j][col] * in_2_[2 * j + 1][col];
  }
  return result;
}

std::vector<std::vector<double>> Model::WinogradMulti(
    std::vector<double>& row_f, std::vector<double>& col_f) {
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

void Model::IfNotEven(std::vector<std::vector<double>>& result) {
  for (int row = 0; row < row_one; ++row)
    for (int col = 0; col < col_two; ++col)
      result[row][col] += in_1_[row][row_two - 1] * in_2_[row_two - 1][col];
}

std::vector<std::vector<double>> Model::Parallel() {
  auto row_f = RowFactorParallel();

  auto col_f = ColFactorParallel();

  auto res = WinogradMultiParallel(row_f, col_f);

  if (!even_) IfNotEvenParallel(res);

  return res;
}

std::vector<double> Model::RowFactorParallel() {
  std::vector<double> result(row_one);
#pragma omp parallel for schedule(dynamic)
  for (int row = 0; row < row_one; ++row) {
    for (int j = 0; j < half_; ++j)
      result[row] += in_1_[row][2 * j] * in_1_[row][2 * j + 1];
  }
  return result;
}

std::vector<double> Model::ColFactorParallel() {
  std::vector<double> result(col_two);
#pragma omp parallel for schedule(dynamic)
  for (int col = 0; col < col_two; ++col) {
    for (int j = 0; j < half_; ++j) {
      result[col] += in_2_[2 * j][col] * in_2_[2 * j + 1][col];
    }
  }
  return result;
}

std::vector<std::vector<double>> Model::WinogradMultiParallel(
    std::vector<double>& row_f, std::vector<double>& col_f) {
  std::vector<std::vector<double>> result(row_one);
  for (auto& row : result) row.resize(col_two);
#pragma omp parallel for schedule(dynamic) collapse(2)
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

void Model::IfNotEvenParallel(std::vector<std::vector<double>>& result) {
#pragma omp parallel for schedule(dynamic) collapse(2)
  for (int row = 0; row < row_one; ++row)
    for (int col = 0; col < col_two; ++col)
      result[row][col] += in_1_[row][row_two - 1] * in_2_[row_two - 1][col];
}

std::vector<std::vector<double>> Model::Pipeline() {
  std::vector<std::vector<double>> result_matrix(row_one);
  for (auto& row : result_matrix) row.resize(col_two);

  TSQueue<int> t1_q;
  TSQueue<int> t2_q;
  TSQueue<int> t3_q;

  auto col_factor = ColFactorParallel();
  auto row_factor = RowFactorParallel();

  std::thread t1([&]() {
// std::cout << "T1 START" << std::endl;
#pragma omp parallel for schedule(dynamic)
    for (int row = 0; row < row_one; ++row) {
      for (int col = 0; col < col_two; ++col) {
        for (auto k = 0; k < half_; ++k) {
          result_matrix[row][col] +=
              (in_1_[row][2 * k] + in_2_[2 * k + 1][col]) *
              (in_1_[row][2 * k + 1] + in_2_[2 * k][col]);
        }
      }
      t1_q.push(1);
    }
    // std::cout << "T1 END" << std::endl;
  });

  std::thread t2([&]() {
    // std::cout << "T2 START" << std::endl;
    for (int row = 0; row < row_one; ++row) {
      t1_q.pop();
      for (int col = 0; col < col_two; ++col) {
        result_matrix[row][col] += -row_factor[row];
      }
      t2_q.push(1);
    }
    // std::cout << "T2 END" << std::endl;
  });

  std::thread t3([&]() {
    // std::cout << "T3 START" << std::endl;
    for (int row = 0; row < row_one; ++row) {
      t2_q.pop();
      for (int col = 0; col < col_two; ++col) {
        result_matrix[row][col] -= col_factor[col];
      }
      t3_q.push(1);
    }
    // std::cout << "T3 END" << std::endl;
  });

  std::thread t4([&]() {
    // std::cout << "T4 START" << std::endl;
    if (even_) return;
    for (int row = 0; row < row_one; ++row) {
      t3_q.pop();
      for (int col = 0; col < col_two; ++col) {
        result_matrix[row][col] +=
            in_1_[row][row_two - 1] * in_2_[row_two - 1][col];
      }
    }
    // std::cout << "T4 END" << std::endl;
  });

  if (t1.joinable()) t1.join();
  if (t2.joinable()) t2.join();
  if (t3.joinable()) t3.join();
  if (t4.joinable()) t4.join();

  return result_matrix;
}

void Model::Load(std::vector<std::vector<double>>& matrix,
                 const std::string& path) {
  std::ifstream in(path);
  std::string temp;
  double x;
  int c = 0;

  while (getline(in, temp)) {
    std::istringstream ss(temp);
    while (ss >> x) {
      matrix[c].emplace_back(x);
    }
    c++;
  }
}

}  // namespace s21