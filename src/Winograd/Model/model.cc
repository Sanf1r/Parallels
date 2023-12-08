#include "model.h"

namespace s21 {

bool Model::BeforeCalculation(int thread_num) {
  row_one_ = in_1_.GetRows();
  col_one_ = in_1_.GetCols();
  row_two_ = in_2_.GetRows();
  col_two_ = in_2_.GetCols();
  if (col_one_ != row_two_) return false;
  omp_set_num_threads(thread_num);
  if (row_two_ % 2 == 0) even_ = true;
  half_ = row_two_ / 2;
  standart_result_.SetRows(row_one_);
  standart_result_.SetCols(col_two_);
  parallel_result_.SetRows(row_one_);
  parallel_result_.SetCols(col_two_);
  pipeline_result_.SetRows(row_one_);
  pipeline_result_.SetCols(col_two_);
  standart_result_.Reserve();
  parallel_result_.Reserve();
  pipeline_result_.Reserve();
  return true;
}

void Model::Standart(int loops) {
  int counter = 0;
  while (counter++ < loops) {
    auto row_f = RowFactor();

    auto col_f = ColFactor();

    WinogradMulti(row_f, col_f);

    if (!even_) IfNotEven();
  }
}

std::vector<double> Model::RowFactor() {
  std::vector<double> result(row_one_);
  for (int row = 0; row < row_one_; ++row) {
    for (int j = 0; j < half_; ++j) {
      result[row] += in_1_(row, 2 * j) * in_1_(row, 2 * j + 1);
    }
  }
  return result;
}

std::vector<double> Model::ColFactor() {
  std::vector<double> result(col_two_);
  for (int col = 0; col < col_two_; ++col) {
    for (int j = 0; j < half_; ++j)
      result[col] += in_2_(2 * j, col) * in_2_(2 * j + 1, col);
  }
  return result;
}

void Model::WinogradMulti(const std::vector<double>& row_f,
                          const std::vector<double>& col_f) {
  for (int row = 0; row < row_one_; ++row) {
    for (int col = 0; col < col_two_; ++col) {
      standart_result_(row, col) = -row_f[row] - col_f[col];
      for (auto k = 0; k < half_; ++k) {
        standart_result_(row, col) +=
            (in_1_(row, 2 * k) + in_2_(2 * k + 1, col)) *
            (in_1_(row, 2 * k + 1) + in_2_(2 * k, col));
      }
    }
  }
}

void Model::IfNotEven() {
  for (int row = 0; row < row_one_; ++row)
    for (int col = 0; col < col_two_; ++col)
      standart_result_(row, col) +=
          in_1_(row, row_two_ - 1) * in_2_(row_two_ - 1, col);
}

void Model::Parallel(int loops) {
  int counter = 0;
  while (counter++ < loops) {
    auto row_f = RowFactorParallel();

    auto col_f = ColFactorParallel();

    WinogradMultiParallel(row_f, col_f);

    if (!even_) IfNotEvenParallel();
  }
}

std::vector<double> Model::RowFactorParallel() {
  std::vector<double> result(row_one_);
#pragma omp parallel for schedule(dynamic)
  for (int row = 0; row < row_one_; ++row) {
    for (int j = 0; j < half_; ++j)
      result[row] += in_1_(row, 2 * j) * in_1_(row, 2 * j + 1);
  }
  return result;
}

std::vector<double> Model::ColFactorParallel() {
  std::vector<double> result(col_two_);
#pragma omp parallel for schedule(dynamic)
  for (int col = 0; col < col_two_; ++col) {
    for (int j = 0; j < half_; ++j) {
      result[col] += in_2_(2 * j, col) * in_2_(2 * j + 1, col);
    }
  }
  return result;
}

void Model::WinogradMultiParallel(const std::vector<double>& row_f,
                                  const std::vector<double>& col_f) {
  std::vector<std::vector<double>> result(row_one_);
  for (auto& row : result) row.resize(col_two_);
#pragma omp parallel for schedule(dynamic) collapse(2)
  for (int row = 0; row < row_one_; ++row) {
    for (int col = 0; col < col_two_; ++col) {
      parallel_result_(row, col) = -row_f[row] - col_f[col];
      for (auto k = 0; k < half_; ++k) {
        parallel_result_(row, col) +=
            (in_1_(row, 2 * k) + in_2_(2 * k + 1, col)) *
            (in_1_(row, 2 * k + 1) + in_2_(2 * k, col));
      }
    }
  }
}

void Model::IfNotEvenParallel() {
#pragma omp parallel for schedule(dynamic) collapse(2)
  for (int row = 0; row < row_one_; ++row)
    for (int col = 0; col < col_two_; ++col)
      parallel_result_(row, col) +=
          in_1_(row, row_two_ - 1) * in_2_(row_two_ - 1, col);
}

void Model::Pipeline(int loops) {
  // omp_set_num_threads(4);
  int counter = 0;
  while (counter++ < loops) {
    pipeline_result_.Clear();
    pipeline_result_.Reserve();

    TSQueue<int> t1_q;
    TSQueue<int> t2_q;
    TSQueue<int> t3_q;

    auto col_factor = ColFactorParallel();
    auto row_factor = RowFactorParallel();

    std::thread t1([&]() {
#pragma omp parallel for schedule(dynamic)
      for (int row = 0; row < row_one_; ++row) {
        for (int col = 0; col < col_two_; ++col) {
          for (auto k = 0; k < half_; ++k) {
            pipeline_result_(row, col) +=
                (in_1_(row, 2 * k) + in_2_(2 * k + 1, col)) *
                (in_1_(row, 2 * k + 1) + in_2_(2 * k, col));
          }
        }
        t1_q.push(1);
      }
    });

    std::thread t2([&]() {
      for (int row = 0; row < row_one_; ++row) {
        t1_q.pop();
        for (int col = 0; col < col_two_; ++col) {
          pipeline_result_(row, col) += -row_factor[row];
        }
        t2_q.push(1);
      }
    });

    std::thread t3([&]() {
      for (int row = 0; row < row_one_; ++row) {
        t2_q.pop();
        for (int col = 0; col < col_two_; ++col) {
          pipeline_result_(row, col) -= col_factor[col];
        }
        t3_q.push(1);
      }
    });

    std::thread t4([&]() {
      if (even_) return;
      for (int row = 0; row < row_one_; ++row) {
        t3_q.pop();
        for (int col = 0; col < col_two_; ++col) {
          pipeline_result_(row, col) +=
              in_1_(row, row_two_ - 1) * in_2_(row_two_ - 1, col);
        }
      }
    });

    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();
    if (t3.joinable()) t3.join();
    if (t4.joinable()) t4.join();
  }
}

bool Model::LoadMatrix(const std::string& f_path, const std::string& s_path) {
  return (LoadLogic(f_path, in_1_) && LoadLogic(s_path, in_2_)) ? true : false;
}

bool Model::LoadLogic(const std::string& path, Matrix& matrix) {
  std::ifstream in(path);
  if (in.bad()) return false;
  std::string temp;
  std::getline(in, temp);
  std::istringstream ss(temp);

  matrix.Clear();

  int rows, cols;

  ss >> rows >> cols;
  if (rows < 1 || cols < 1) return false;

  matrix.SetRows(rows);
  matrix.SetCols(cols);
  matrix.Reserve();

  for (int i = 0; i < matrix.GetRows(); ++i) {
    std::getline(in, temp);
    std::istringstream ss_in(temp);

    double x;
    int c = 0;
    while (ss_in >> x || !ss_in.eof()) {
      if (ss_in.fail()) return false;
      matrix.Push(x);
      c++;
    }

    if (c != matrix.GetCols()) return false;
  }
  return true;
}

void Model::GenerateMatrix(int f_rows, int f_cols, int s_rows, int s_cols) {
  GenerateLogic(f_rows, f_cols, in_1_);
  GenerateLogic(s_rows, s_cols, in_2_);
}

void Model::GenerateLogic(int rows, int cols, Matrix& matrix) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_real_distribution<double> gen(-100, 100);
  matrix.Clear();
  matrix.SetRows(rows);
  matrix.SetCols(cols);
  matrix.Reserve();
  for (int i = 0; i < rows * cols; ++i) {
    matrix.Push(gen(rng));
  }
}

}  // namespace s21