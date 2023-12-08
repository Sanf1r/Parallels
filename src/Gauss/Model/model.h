#ifndef PARALLELS_SRC_GAUSS_MODEL_MODEL_H_
#define PARALLELS_SRC_GAUSS_MODEL_MODEL_H_

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <thread>
#include <vector>

namespace s21 {

class Model {
 public:
  bool LoadGraphFromFile(const std::string &filename);
  int SolvingSystemsLinearEquations(int loop);
  int SolvingSystemsLinearEquationsParallels(int loop);
  std::vector<double> GetResult();
  std::vector<double> GetResultParallels();
  bool GetResultCorrect();

 private:
  bool result_correct_ = true;
  std::vector<double> final_result_;
  std::vector<double> final_result_par_;
  std::vector<std::vector<double>> matrix_origin_;
  std::vector<std::vector<double>> matrix_;
  std::vector<std::vector<double>> matrix_par_;

  int size_ = 0;
  int threads_number_ = 10;

  // void PrintMatrix(std::vector<std::vector<double>> &matrix);

  void DividingLineToNumber(std::vector<double> &line, double number);
  void MultiplyLineToNumber(const std::vector<double> &line, double number,
                            std::vector<double> &result);
  void AddingLines(const std::vector<double> &line1,
                   const std::vector<double> &line2,
                   std::vector<double> &result);
  void SwapLinesMatrix(std::vector<std::vector<double>> &matrix, int line,
                       int line_2);
  bool FixZeroDiagonalLine(std::vector<std::vector<double>> &matrix,
                           int line_number);

  void CalculateLinesUp(std::vector<std::vector<double>> &matrix,
                        int mainLineNumber, int start_point, int end_point);
  void CalculateLinesDown(std::vector<std::vector<double>> &matrix,
                          int mainLineNumber, int start_point, int end_point);

  int MatrixUpdateDown(std::vector<std::vector<double>> &matrix);
  void MatrixUpdateUp(std::vector<std::vector<double>> &matrix);

  int MatrixUpdateDownParallels(std::vector<std::vector<double>> &matrix);
  void MatrixUpdateUpParallels(std::vector<std::vector<double>> &matrix);

  /*
   int MatrixUpdateDownOmp(std::vector<std::vector<double>> &matrix);
   void MatrixUpdateUpOmp(std::vector<std::vector<double>> &matrix);
  */

  void CheckResult(const std::vector<std::vector<double>> &matrix);
};

}  // namespace s21

#endif  //  PARALLELS_SRC_GAUSS_MODEL_MODEL_H_
