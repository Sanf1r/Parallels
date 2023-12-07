#ifndef SRC_CONTROLLER_CONTROLLER_H_
#define SRC_CONTROLLER_CONTROLLER_H_

#include "../Model/model.h"

namespace s21 {

class Controller {
 public:
  Controller() = delete;
  explicit Controller(Model* m) : model_(m){};

  void BeforeCalculation(int thread_num) { model_->BeforeCalculation(thread_num); }

  void Standart(int loops) { model_->Standart(loops); }
  void Parallel(int loops) { model_->Parallel(loops); }
  void Pipeline(int loops) { model_->Pipeline(loops); }

  bool LoadMatrix(const std::string& f_path, const std::string& s_path) {
    return model_->LoadMatrix(f_path, s_path);
  }

 private:
  Model* model_;
};
}  // namespace s21

#endif  //  SRC_CONTROLLER_CONTROLLER_H_
