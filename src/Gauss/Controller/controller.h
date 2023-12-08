#ifndef PARALLELS_SRC_GAUSS_CONTROLLER_CONTROLLER_H_
#define PARALLELS_SRC_GAUSS_CONTROLLER_CONTROLLER_H_

#include "../Model/model.h"

namespace s21 {

class Controller {
 public:
  Controller() = delete;
  explicit Controller(Model* m) : model_(m){};

  bool SolvingSystemsLinearEquations(int loops) {
    return model_->SolvingSystemsLinearEquations(loops);
  }

  int SolvingSystemsLinearEquationsParallels(int loops) {
    return model_->SolvingSystemsLinearEquationsParallels(loops);
  }

  int LoadGraphFromFile(const std::string& path) {
    return model_->LoadGraphFromFile(path);
  }

  std::vector<double> GetResult() { return model_->GetResult(); }

  std::vector<double> GetResultParallels() {
    return model_->GetResultParallels();
  }

  bool GetResultCorrect() { return model_->GetResultCorrect(); }

 private:
  Model* model_;
};

}  // namespace s21

#endif  //  PARALLELS_SRC_GAUSS_CONTROLLER_CONTROLLER_H_
