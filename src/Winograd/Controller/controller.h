#ifndef PARALLELS_SRC_WINOGRAD_CONTROLLER_CONTROLLER_H_
#define PARALLELS_SRC_WINOGRAD_CONTROLLER_CONTROLLER_H_

#include "../Model/model.h"

namespace s21 {

class Controller {
 public:
  Controller() = delete;
  explicit Controller(Model* m) : model_(m){};

  bool BeforeCalculation(int thread_num) {
    return model_->BeforeCalculation(thread_num);
  }

  void Standart(int loops) { model_->Standart(loops); }
  void Parallel(int loops) { model_->Parallel(loops); }
  void Pipeline(int loops) { model_->Pipeline(loops); }

  void PrintStandartRes() { model_->PrintStandartRes(); }
  void PrintParallelRes() { model_->PrintParallelRes(); }
  void PrintPipelineRes() { model_->PrintPipelineRes(); }

  bool LoadMatrix(const std::string& f_path, const std::string& s_path) {
    return model_->LoadMatrix(f_path, s_path);
  }

  void GenerateMatrix(int f_rows, int f_cols, int s_rows, int s_cols) {
    model_->GenerateMatrix(f_rows, f_cols, s_rows, s_cols);
  }

 private:
  Model* model_;
};
}  // namespace s21

#endif  //  PARALLELS_SRC_WINOGRAD_CONTROLLER_CONTROLLER_H_
