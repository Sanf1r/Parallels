#ifndef A2_SIMPLENAVIGATOR_SRC_CONTROLLER_CONTROLLER_H_
#define A2_SIMPLENAVIGATOR_SRC_CONTROLLER_CONTROLLER_H_

#include "../Model/model.h"

namespace s21 {

class Controller {
 public:
  Controller() = delete;
  explicit Controller(Model* m) : model_(m){};

  TsmResult SolveTravelingSalesmanProblem() {
    return model_->SolveTravelingSalesmanProblem();
  }

  TsmResult SolveTravelingSalesmanProblemParallel() {
    return model_->SolveTravelingSalesmanProblemParallel();
  }

  bool LoadGraphFromFile(const std::string& path) {
    return model_->LoadGraphFromFile(path);
  }

  bool UndirectedCheck() { return model_->UndirectedCheck(); }

  int GetSize() { return model_->GetSize(); }

 private:
  Model* model_;
};
}  // namespace s21

#endif  //  A2_SIMPLENAVIGATOR_SRC_CONTROLLER_CONTROLLER_H_
