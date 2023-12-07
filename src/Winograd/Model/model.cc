#include "model.h"

namespace s21 {

void Model::BeforeCalculation(int thread_num) {
  omp_set_num_threads(thread_num);
  if (row_two % 2 == 0) even_ = true;
  half_ = row_two / 2;
  standart_result_.clear();
  parallel_result_.clear();
  pipeline_result_.clear();
}

void Model::Standart(int loops) {
  int counter = 0;
  while (counter++ < loops) {
    auto row_f = RowFactor();

    auto col_f = ColFactor();

    auto res = WinogradMulti(row_f, col_f);

    if (!even_) IfNotEven(res);

    standart_result_ = std::move(res);
  }
}

std::vector<double> Model::RowFactor() {
  std::vector<double> result(row_one);
  for (int row = 0; row < row_one; ++row) {
    for (int j = 0; j < half_; ++j) {
      result[row] += in_1_[row][2 * j] * in_1_[row][2 * j + 1];
    }
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

void Model::Parallel(int loops) {
  int counter = 0;
  while (counter++ < loops) {
    auto row_f = RowFactorParallel();

    auto col_f = ColFactorParallel();

    auto res = WinogradMultiParallel(row_f, col_f);

    if (!even_) IfNotEvenParallel(res);

    parallel_result_ = std::move(res);
  }
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

void Model::Pipeline(int loops) {
  int counter = 0;
  while (counter++ < loops) {
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

    pipeline_result_ = std::move(result_matrix);
  }
}

bool Model::LoadMatrix(const std::string& f_path, const std::string& s_path) {
  return (LoadLogicFirst(f_path, in_1_) && LoadLogicSecond(s_path, in_2_))
             ? true
             : false;
}

bool Model::LoadLogicFirst(const std::string& path,
                           std::vector<std::vector<double>>& matrix) {
  std::ifstream in(path);
  if (in.bad()) return false;
  std::string temp;
  std::getline(in, temp);
  std::istringstream ss(temp);

  matrix.clear();

  ss >> row_one >> col_one;
  if (row_one < 1 || col_one < 1) return false;

  matrix.resize(row_one);
  for (auto& m : matrix) m.reserve(col_one);

  for (int i = 0; i < row_one; ++i) {
    std::getline(in, temp);
    std::istringstream ss_in(temp);

    double x;
    int c = 0;
    while (ss_in >> x || !ss_in.eof()) {
      if (ss_in.fail()) return false;
      matrix[i].emplace_back(x);
      c++;
    }

    if (c != col_one) return false;
  }
  return true;
}

bool Model::LoadLogicSecond(const std::string& path,
                            std::vector<std::vector<double>>& matrix) {
  std::ifstream in(path);
  if (in.bad()) return false;
  std::string temp;
  std::getline(in, temp);
  std::istringstream ss(temp);

  matrix.clear();

  ss >> row_two >> col_two;
  if (row_two < 1 || col_two < 1) return false;

  matrix.resize(row_two);
  for (auto& m : matrix) m.reserve(col_two);

  for (int i = 0; i < row_two; ++i) {
    std::getline(in, temp);
    std::istringstream ss_in(temp);

    double x;
    int c = 0;
    while (ss_in >> x || !ss_in.eof()) {
      if (ss_in.fail()) return false;
      matrix[i].emplace_back(x);
      c++;
    }

    if (c != col_two) return false;
  }
  return true;
}

}  // namespace s21