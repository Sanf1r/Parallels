#include "model.h"

namespace s21 {

bool Model::LoadGraphFromFile(const std::string &filename) {
  std::ifstream file(filename);
  if (file.bad()) {
    return false;
  }

  std::string temp;
  std::getline(file, temp);
  std::istringstream ss(temp);
  matrix_origin_.clear();

  ss >> size_;
  if (size_ < 1) {
    return false;
  }

  matrix_origin_.resize(size_);
  for (int i = 0; i < size_; ++i) {
    std::getline(file, temp);
    std::istringstream ss_in(temp);

    int x;
    int c = 0;
    while (ss_in >> x || !ss_in.eof()) {
      if (ss_in.fail()) {
        return false;
      }

      matrix_origin_[i].push_back(x);
      c++;
    }
    c--;
    if (c != size_) {
      return false;
    }
  }
  return true;
}

/*
void Model::PrintMatrix(std::vector<std::vector<double>> &matrix) {
  for (size_t i = 0; i < matrix.size(); ++i) {
    for (size_t j = 0; j < matrix[i].size(); ++j) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}
*/

void Model::DividingLineToNumber(std::vector<double> &line, double number) {
  for (size_t i = 0; i < line.size(); ++i) {
    line[i] /= number;
  }
}

void Model::MultiplyLineToNumber(const std::vector<double> &line, double number,
                                 std::vector<double> &result) {
  for (size_t i = 0; i < line.size(); ++i) {
    result[i] = line[i] * number;
  }
}

void Model::AddingLines(const std::vector<double> &line1,
                        const std::vector<double> &line2,
                        std::vector<double> &result) {
  for (size_t i = 0; i < line1.size(); ++i) {
    result[i] = line1[i] + line2[i];
  }
}

void Model::SwapLinesMatrix(std::vector<std::vector<double>> &matrix,
                            int line_1, int line_2) {
  std::vector<double> tmp_line_start;
  std::vector<double> tmp_line_end;
  int line_start = std::min(line_1, line_2);
  int line_end = std::max(line_1, line_2);

  for (size_t i = 0; i < matrix.size(); ++i) {
    if (line_start == (int)i) {
      tmp_line_start = matrix[i];
      for (size_t j = i; j < matrix.size(); ++j, ++i) {
        if (line_end == (int)j) {
          tmp_line_end = matrix[i];
          matrix[j] = tmp_line_start;
          for (int k = j; k >= 0; --k) {
            if (line_start == k) {
              matrix[k] = tmp_line_end;
            }
          }
        }
      }
    }
  }
}

bool Model::FixZeroDiagonalLine(std::vector<std::vector<double>> &matrix,
                                int line_number) {
  for (size_t i = line_number + 1; i < matrix.size(); ++i) {
    if (matrix[i][line_number] != 0) {
      SwapLinesMatrix(matrix, line_number, i);
      return true;
    }
  }
  return false;
}

void Model::CalculateLinesUp(std::vector<std::vector<double>> &matrix,
                             int mainLineNumber, int start_point,
                             int end_point) {
  int matrix_size = matrix.size();
  for (size_t i = start_point; (int)i >= end_point; --i) {
    if (matrix[i][mainLineNumber] != 0) {
      std::vector<double> tmp(matrix_size + 1);
      std::vector<double> tmp2(matrix_size + 1);
      MultiplyLineToNumber(matrix[mainLineNumber],
                           matrix[i][mainLineNumber] * -1, tmp);
      AddingLines(tmp, matrix[i], tmp2);
      matrix[i] = tmp2;
    }
  }
}

void Model::CalculateLinesDown(std::vector<std::vector<double>> &matrix,
                               int mainLineNumber, int start_point,
                               int end_point) {
  int matrix_size = matrix.size();
  for (int i = start_point; i < end_point; ++i) {
    if (matrix[i][mainLineNumber] != 0) {
      std::vector<double> tmp(matrix_size + 1);
      std::vector<double> tmp2(matrix_size + 1);
      MultiplyLineToNumber(matrix[mainLineNumber],
                           matrix[i][mainLineNumber] * -1, tmp);
      AddingLines(tmp, matrix[i], tmp2);
      matrix[i] = tmp2;
    }
  }
}

int Model::MatrixUpdateDown(std::vector<std::vector<double>> &matrix) {
  int matrix_size = matrix.size();
  for (int h = 0; h < matrix_size - 1; ++h) {
    if (matrix[h][h] == 0) {
      if (!FixZeroDiagonalLine(matrix, 0)) {
        return 0;
      }
    }
    if (matrix[h][h] != 1) {
      DividingLineToNumber(matrix[h], matrix[h][h]);
    }
    CalculateLinesDown(matrix, h, h + 1, matrix_size);
  }
  matrix_size--;
  if (matrix[matrix_size][matrix_size] != 1) {
    DividingLineToNumber(matrix[matrix_size], matrix[matrix_size][matrix_size]);
  }

  return 1;
}

void Model::MatrixUpdateUp(std::vector<std::vector<double>> &matrix) {
  for (int h = matrix.size() - 1; h > 0; --h) {
    CalculateLinesUp(matrix, h, h - 1, 0);
  }
}

int Model::MatrixUpdateDownParallels(std::vector<std::vector<double>> &matrix) {
  std::vector<std::thread> threads;
  int matrix_size = matrix.size();
  int step = matrix_size;
  if (threads_number_ < matrix_size) {
    step = matrix_size / threads_number_;
    step += 1;
  }

  int start_point;
  int end_point;
  for (int h = 0; h < matrix_size - 1; ++h) {
    if (matrix[h][h] == 0) {
      if (!FixZeroDiagonalLine(matrix, 0)) {
        return 0;
      }
    }
    if (matrix[h][h] != 1) {
      DividingLineToNumber(matrix[h], matrix[h][h]);
    }
    bool skip = false;
    for (int i = 1; i < matrix_size; i += step) {
      start_point = h + i;
      if (start_point > matrix_size) start_point = matrix_size;
      if (start_point == matrix_size) {
        if (skip) {
          break;
        } else {
          skip = true;
        }
      }
      end_point = start_point + step;
      if (end_point > matrix_size) end_point = matrix_size;
      if (end_point == matrix_size) skip = true;

      threads.push_back(std::thread(&s21::Model::CalculateLinesDown, this,
                                    std::ref(matrix), h, start_point,
                                    end_point));
    }
    for (auto &thread : threads) {
      thread.join();
    }
    threads.clear();
  }
  matrix_size--;
  if (matrix[matrix_size][matrix_size] != 1) {
    DividingLineToNumber(matrix[matrix_size], matrix[matrix_size][matrix_size]);
  }
  return 1;
}

void Model::MatrixUpdateUpParallels(std::vector<std::vector<double>> &matrix) {
  std::vector<std::thread> threads;
  int matrix_size = matrix.size();
  int step = matrix_size;
  if (threads_number_ < matrix_size) {
    step = matrix_size / threads_number_;
    step += 1;
  }

  int start_point;
  int end_point;
  for (int h = matrix_size - 1; h > 0; --h) {
    bool skip = false;
    for (int i = 1; i < matrix_size; i += step) {
      start_point = h - i;
      if (start_point < 0) start_point = 0;
      if (start_point == 0) {
        if (skip) {
          break;
        } else {
          skip = true;
        }
      }
      end_point = start_point - step + 1;
      if (end_point < 0) end_point = 0;
      if (end_point == 0) skip = true;
      threads.push_back(std::thread(&s21::Model::CalculateLinesUp, this,
                                    std::ref(matrix), h, start_point,
                                    end_point));
    }
    for (auto &thread : threads) {
      thread.join();
    }
    threads.clear();
  }
}

void Model::CheckResult(const std::vector<std::vector<double>> &matrix) {
  result_correct_ = true;
  for (size_t i = 0; i < matrix.size(); ++i) {
    if (matrix[i][i] != 1) {
      result_correct_ = false;
      break;
    }
  }
}

std::vector<double> Model::GetResult() { return final_result_; }

std::vector<double> Model::GetResultParallels() { return final_result_par_; }

bool Model::GetResultCorrect() { return result_correct_; }

/*
int Model::MatrixUpdateDownOmp(std::vector<std::vector<double>> &matrix) {
  int step = 21;
  int start_point;
  int end_point;
  int matrix_size = matrix.size();
  for (int h = 0; h < matrix_size - 1; ++h) {
    if (matrix[h][h] == 0) {
      if (!FixZeroDiagonalLine(matrix, 0)) {
        return 0;
      }
    }
    if (matrix[h][h] != 1) {
      DividingLineToNumber(matrix[h], matrix[h][h]);
    }
    bool skip = false;
#pragma omp parallel for schedule(dynamic)
    for (int i = 1; i < matrix_size; i += step) {
      start_point = h + i;
      if (start_point > matrix_size) start_point = matrix_size;
      if (start_point == matrix_size) {
        if (skip) {
          skip = false;
        } else {
          skip = true;
        }
      }
      end_point = start_point + step;
      if (end_point > matrix_size) end_point = matrix_size;
      if (end_point == matrix_size) skip = true;

      CalculateLinesDown(matrix, h, start_point, end_point);
    }
  }
  matrix_size--;
  if (matrix[matrix_size][matrix_size] != 1) {
    DividingLineToNumber(matrix[matrix_size], matrix[matrix_size][matrix_size]);
  }
  return 1;
}

void Model::MatrixUpdateUpOmp(std::vector<std::vector<double>> &matrix) {
  int step = 21;
  int start_point;
  int end_point;
  int matrix_size = matrix.size();
  for (int h = matrix_size - 1; h > 0; --h) {
    bool skip = false;
#pragma omp parallel for schedule(dynamic)
    for (int i = 1; i < matrix_size; i += step) {
      start_point = h - i;
      if (start_point < 0) start_point = 0;
      if (start_point == 0) {
        if (skip) {
          skip = false;
        } else {
          skip = true;
        }
      }
      end_point = start_point - step + 1;
      if (end_point < 0) end_point = 0;
      if (end_point == 0) skip = true;

      CalculateLinesUp(matrix, h, start_point, end_point);
    }
  }
}
*/

int Model::SolvingSystemsLinearEquations(int loop) {
  for (int i = 0; i < loop; ++i) {
    matrix_ = matrix_origin_;
    if (matrix_[0][0] == 0) {
      if (!FixZeroDiagonalLine(matrix_, 0)) {
        return 0;
      }
    }

    if (MatrixUpdateDown(matrix_)) {
      result_correct_ = false;
      break;
    }
    MatrixUpdateUp(matrix_);
  }

  CheckResult(matrix_);
  if (!result_correct_) {
    return 0;
  }

  final_result_.clear();
  for (size_t i = 0; i < matrix_.size(); ++i) {
    final_result_.push_back(matrix_[i][matrix_[0].size() - 1]);
  }
  return 1;
}

int Model::SolvingSystemsLinearEquationsParallels(int loop) {
  for (int i = 0; i < loop; ++i) {
    matrix_par_ = matrix_origin_;
    if (matrix_par_[0][0] == 0) {
      if (!FixZeroDiagonalLine(matrix_par_, 0)) {
        return 0;
      }
    }

    if (MatrixUpdateDownParallels(matrix_par_)) {
      result_correct_ = false;
      break;
    }
    MatrixUpdateUpParallels(matrix_par_);
    // if(MatrixUpdateDownOmp(matrix_par_)) {
    //   result_correct_ = false;
    //   break;
    // }
    // MatrixUpdateUpOmp(matrix_par_);
  }

  CheckResult(matrix_par_);
  if (!result_correct_) {
    return 0;
  }

  final_result_par_.clear();
  for (size_t i = 0; i < matrix_par_.size(); ++i) {
    final_result_par_.push_back(matrix_par_[i][matrix_par_[0].size() - 1]);
  }

  return 1;
}

}  // namespace s21
